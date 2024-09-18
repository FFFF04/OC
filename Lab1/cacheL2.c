#include "cacheL2.h"


uint8_t DRAM[DRAM_SIZE];
uint32_t time = 0;

CacheL1 SimpleCacheL1;  // Cache L1
CacheL2 SimpleCacheL2;  // Cache L2

/*********************** Funções Auxiliares *************************/

// Inicializa a cache L1
void initCacheL1() {
    SimpleCacheL1.init = 0;
}

// Inicializa a cache L2
void initCacheL2() {
    SimpleCacheL2.init = 0;
}

// Acesso à DRAM
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {
    if (address >= DRAM_SIZE - BLOCK_SIZE + 1) {
        exit(-1);  // Endereço fora do intervalo
    }
    if (mode == MODE_READ) {
        memcpy(data, &DRAM[address], BLOCK_SIZE);
        time += DRAM_READ_TIME;
    } else if (mode == MODE_WRITE) {
        memcpy(&DRAM[address], data, BLOCK_SIZE);
        time += DRAM_WRITE_TIME;
    }
}

/*********************** Funções da Cache *************************/

// Acesso à cache L1
int accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t Tag, Index, MemAddress;
    uint8_t TempBlock[BLOCK_SIZE];

    if (SimpleCacheL1.init == 0) {
        for (int i = 0; i < L1_LINES; i++) {
            SimpleCacheL1.lines[i].Valid = 0;
        }
        SimpleCacheL1.init = 1;
    }

    /** 
    #TODO
    Tag = address >> 3;             // como obter a tag?
    Index = address << 3;           // Como obter o indice?
    */

    CacheLine *Line = &SimpleCacheL1.lines[Index];

    MemAddress = (address >> 3) << 3;  // Alinha o endereço do bloco

    // Cache Miss: se a linha não for válida ou o Tag não coincidir
    if (!Line->Valid || Line->Tag != Tag) {
        return 0;  // Indica miss na L1
    }

    // Leitura
    if (mode == MODE_READ) {
        if ((address % 8) == 0) {  // Palavra par
            memcpy(data, &(Line->Data[0]), WORD_SIZE);
        } else {                   // Palavra ímpar
            memcpy(data, &(Line->Data[WORD_SIZE]), WORD_SIZE);
        }
        time += L1_READ_TIME;
    }

    // Escrita
    if (mode == MODE_WRITE) {
        if ((address % 8) == 0) {  // Palavra par
            memcpy(&(Line->Data[0]), data, WORD_SIZE);
        } else {                   // Palavra ímpar
            memcpy(&(Line->Data[WORD_SIZE]), data, WORD_SIZE);
        }
        Line->Dirty = 1;
        time += L1_WRITE_TIME;
    }

    return 1;  // Indica hit na L1
}

// Acesso à cache L2
int accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t Tag, Index, MemAddress;
    uint8_t TempBlock[BLOCK_SIZE];

    if (SimpleCacheL2.init == 0) {
        for (int i = 0; i < L2_LINES; i++) {
            SimpleCacheL2.lines[i].Valid = 0;
        }
        SimpleCacheL2.init = 1;
    }

    /** 
    #TODO
    Tag = address >> 3;             // como obter a tag?
    Index = address << 3;  // Como obter o indice?
    */
    CacheLine *Line = &SimpleCacheL2.lines[Index];

    MemAddress = (address >> 3) << 3;  // Alinha o endereço do bloco

    // Cache Miss: se a linha não for válida ou o Tag não coincidir
    if (!Line->Valid || Line->Tag != Tag) {
        // Carrega o bloco da DRAM
        accessDRAM(MemAddress, TempBlock, MODE_READ);
        memcpy(Line->Data, TempBlock, BLOCK_SIZE);
        Line->Valid = 1;
        Line->Tag = Tag;
        Line->Dirty = 0;
        return 0;  // Indica miss na L2
    }

    // Leitura
    if (mode == MODE_READ) {
        if ((address % 8) == 0) {  // Palavra par
            memcpy(data, &(Line->Data[0]), WORD_SIZE);
        } else {                   // Palavra ímpar
            memcpy(data, &(Line->Data[WORD_SIZE]), WORD_SIZE);
        }
        time += L2_READ_TIME;
    }

    // Escrita
    if (mode == MODE_WRITE) {
        if ((address % 8) == 0) {  // Palavra par
            memcpy(&(Line->Data[0]), data, WORD_SIZE);
        } else {                   // Palavra ímpar
            memcpy(&(Line->Data[WORD_SIZE]), data, WORD_SIZE);
        }
        Line->Dirty = 1;
        time += L2_WRITE_TIME;
    }

    return 1;  // Indica hit na L2
}

// Função principal para ler e escrever, considerando a hierarquia de cache L1 e L2
void read(uint32_t address, uint8_t *data) {
    // Acessar L1
    if (!accessL1(address, data, MODE_READ)) {
        // Se ocorrer um miss na L1, acessar L2
        if (!accessL2(address, data, MODE_READ)) {
            // Se ocorrer um miss na L2, carregar da DRAM
            accessDRAM(address, data, MODE_READ); //TODO PODE SER QUE SE TENHA Q REMOVER ISTO PQ A NO ACESSO À CACHE 2 JA ACESSAMOS A DRAM
        }
        // Colocar o bloco da L2 para a L1 (write-back)
        accessL1(address, data, MODE_WRITE);
    }
}

void write(uint32_t address, uint8_t *data) {
    // Acessar L1
    if (!accessL1(address, data, MODE_WRITE)) {
        // Se ocorrer um miss na L1, acessar L2
        if (!accessL2(address, data, MODE_WRITE)) {
            // Se ocorrer um miss na L2, carregar da DRAM
            accessDRAM(address, data, MODE_READ);
        }
        // Colocar o bloco da L2 para a L1 (write-back)
        accessL1(address, data, MODE_WRITE);
    }
}
