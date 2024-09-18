#include "cacheL2.h"


uint8_t DRAM[DRAM_SIZE];
uint32_t time = 0;

CacheL1 SimpleCacheL1;  // Cache L1
CacheL2 SimpleCacheL2;  // Cache L2

/*********************** Funções Auxiliares *************************/

void resetTime() { time = 0; }

uint32_t getTime() { return time; }

// Inicializa both caches
void initCache() {
    SimpleCacheL1.init = 0;
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

void write_L1(uint32_t address, uint8_t *data){
    uint32_t index, Tag, Offset;

    /* Calcula o índice e a tag e offset*/
    Offset = address & 0x3F;
    index = (address >> 6) & 0xFF;
    Tag = address >> 14;

    CacheLine *Line = &SimpleCacheL1.lines[index];

    memcpy(&(Line->Data[Offset]), data, WORD_SIZE);
    time += L1_WRITE_TIME;

    Line->Valid = 1;
    Line->Tag = Tag;
    Line->Dirty = 1; // Marca a linha como suja
}

// Acesso à cache L1
int accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t index, Tag, Offset;
    // uint8_t TempBlock[BLOCK_SIZE];

    /* Init cache */
    if (SimpleCacheL1.init == 0) {
        for (int i = 0; i < NUM_LINES_L1; i++) {
            SimpleCacheL1.lines[i].Valid = 0;
            SimpleCacheL1.lines[i].Dirty = 0;
            SimpleCacheL1.lines[i].Tag = 0;
        }
        SimpleCacheL1.init = 1;
    }
    /* Calcula o índice e a tag e offset*/
    Offset = address & 0x3F;
    index = (address >> 6) & 0xFF;
    Tag = address >> 14;

    // Address - offset porque queremos ir para o inicio do bloco
    // MemAddress = address - Offset;

    CacheLine *Line = &SimpleCacheL1.lines[index];

    /* Acesso à Cache */
    if (!Line->Valid || Line->Tag != Tag)  // Cache miss
        return 0;

    /* Leitura de dados */
    if (mode == MODE_READ) {

        memcpy(data, &(Line->Data[Offset]), WORD_SIZE);
        time += L1_READ_TIME;
    }
    /* Escrita de dados */
    if (mode == MODE_WRITE) {
        write_L1(address, data);
        write_L2;
    }
    return 1;
}

// Acesso à cache L2
int accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t index, Tag, MemAddress, Offset;
    uint8_t TempBlock[BLOCK_SIZE];

    /* Init cache */
    if (SimpleCacheL2.init == 0) {
        for (int i = 0; i < NUM_LINES_L2; i++) {
            SimpleCacheL2.lines[i].Valid = 0;
            SimpleCacheL2.lines[i].Dirty = 0;
            SimpleCacheL2.lines[i].Tag = 0;
        }
        SimpleCacheL2.init = 1;
    }
    /* Calcula o índice e a tag e offset*/
    Offset = address & 0x3F;
    index = (address >> 6) & 0x1FF;
    Tag = address >> 15;

    // Address - offset porque queremos ir para o inicio do bloco
    MemAddress = address - Offset;

    CacheLine *Line = &SimpleCacheL2.lines[index];

    // Cache Miss: se a linha não for válida ou o Tag não coincidir
    if (!Line->Valid || Line->Tag != Tag) {
        // Carrega o bloco da DRAM
        accessDRAM(MemAddress, TempBlock, MODE_READ);
        
        // Se a linha estiver suja, escreve o bloco antigo de volta na DRAM
        if (Line->Valid && Line->Dirty) {
            accessDRAM(MemAddress, Line->Data, MODE_WRITE);
        }

        // Substitui o bloco na linha de cache
        memcpy(Line->Data, TempBlock, BLOCK_SIZE);
        Line->Valid = 1;
        Line->Tag = Tag;
        Line->Dirty = 0; // Novo bloco não é sujo inicialmente
        // return 0;  // Indica miss na L2
    }

    // Leitura
    if (mode == MODE_READ) {

        memcpy(data, &(Line->Data[Offset]), WORD_SIZE);
        time += L2_READ_TIME;
    }

    // Escrita
    if (mode == MODE_WRITE) {

        memcpy(&(Line->Data[Offset]), data, WORD_SIZE);
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
            // accessDRAM(address, data, MODE_READ); //TODO PODE SER QUE SE TENHA Q REMOVER ISTO PQ A NO ACESSO À CACHE 2 JA ACESSAMOS A DRAM
        }
        // Colocar o bloco da L2 para a L1 (write-back)
        write_L1(address, data);
    }
}

void write(uint32_t address, uint8_t *data) {
    // Acessar L1
    if (!accessL1(address, data, MODE_WRITE)) {
        // Se ocorrer um miss na L1, acessar L2
        if (!accessL2(address, data, MODE_WRITE)) {
            // Se ocorrer um miss na L2, carregar da DRAM
            // accessDRAM(address, data, MODE_READ);
        }
        // Colocar o bloco da L2 para a L1 (write-back)
        write_L1(address, data);
    }
}
