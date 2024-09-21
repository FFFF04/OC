#include "L3Cache.h"


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


// Acesso à cache L1
void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t index, Tag, Offset,MemAddress;
    uint8_t TempBlock[BLOCK_SIZE];

    /* Init cache */
    if (SimpleCacheL1.init == 0) {
        for (int i = 0; i < NUM_LINES_L1; i++) {
            SimpleCacheL1.lines[i].Valid = 0;
            SimpleCacheL1.lines[i].Dirty = 0;
            SimpleCacheL1.lines[i].Tag = 0;

            for (int j = 0; j < BLOCK_SIZE; j+=WORD_SIZE)
              SimpleCacheL1.lines[i].Data[j] = 0;
        }
        SimpleCacheL1.init = 1;
    }
    /* Calcula o índice e a tag e offset*/
    Offset = address & 0x3F;
    index = (address >> 6) & 0xFF;
    Tag = address >> 14;

    // Address - offset porque queremos ir para o inicio do bloco
    MemAddress = address - Offset;

    CacheLine *Line = &SimpleCacheL1.lines[index];

    /* Acesso à Cache */
    // Cache miss
    if (!Line->Valid || Line->Tag != Tag){
        // Carrega o bloco da DRAM
        accessL2(MemAddress,TempBlock,MODE_READ);
        
        // Se a linha estiver suja, escreve o bloco antigo de volta na DRAM
        if (Line->Valid && Line->Dirty) {
            accessL2(MemAddress, Line->Data, MODE_WRITE);
        }

        // Substitui o bloco na linha de cache
        memcpy(Line->Data, TempBlock, BLOCK_SIZE);
        Line->Valid = 1;
        Line->Tag = Tag;
        Line->Dirty = 0; // Novo bloco não é sujo inicialmente
    }  
        
    /* Leitura de dados */
    if (mode == MODE_READ) {

        memcpy(data, &(Line->Data[Offset]), WORD_SIZE);
        time += L1_READ_TIME;
    }
    /* Escrita de dados */
    if (mode == MODE_WRITE) {
        memcpy(&(Line->Data[Offset]), data, WORD_SIZE);
        time += L1_WRITE_TIME;

        Line->Dirty = 1; // Marca a linha como suja
    }
}

// Acesso à cache L2
void accessL2(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t index, Tag, MemAddress, Offset;
    uint8_t TempBlock[BLOCK_SIZE];

    /* Init cache */
    if (SimpleCacheL2.init == 0) {
        for (int i = 0; i < (NUM_LINES_L2/2); i++) {
            SimpleCacheL2.sets[i].lines[0].Valid = 0;
            SimpleCacheL2.sets[i].lines[0].Dirty = 0;
            SimpleCacheL2.sets[i].lines[0].Tag = 0;

            SimpleCacheL2.sets[i].lines[1].Valid = 0;
            SimpleCacheL2.sets[i].lines[1].Dirty = 0;
            SimpleCacheL2.sets[i].lines[1].Tag = 0;

            SimpleCacheL2.sets[i].LRU = 0;

            for (int k = 0; k < BLOCK_SIZE; k+=WORD_SIZE){
                SimpleCacheL2.sets[i].lines[0].Data[k] = 0;
                SimpleCacheL2.sets[i].lines[1].Data[k] = 0;
            }
        }
        SimpleCacheL2.init = 1;
    }
    /* Calcula o índice e a tag e offset*/
    Offset = address & 0x3F;
    index = (address >> 6) & 0x1FF;
    Tag = address >> 15;

    /*
    Vamos ter 128 sets.------------------------Tiago: não seriam 256 sets? 256*2 =512. Para manter o mesmo size
    Temos de alterar a estrutura da Cache2
    Fazemos um ciclo a percurrer os 128 sets para localizar em que Set esta o address
    A partir dai tiramos a linha 
    */

    // Address - offset porque queremos ir para o inicio do bloco
    MemAddress = address - Offset;

    dataSet *Set = &SimpleCacheL2.sets[index];

    //Verificar cache hit no set
    for(int i = 0; i < 2; i++){
        
        if(Set->lines[i].Valid && Set->lines[i].Tag==Tag){
            // Leitura
            if (mode == MODE_READ) {

                memcpy(data, &(Set->lines[i].Data[Offset]), WORD_SIZE);
                time += L2_READ_TIME;
            }
            // Escrita
            if (mode == MODE_WRITE) {

                memcpy(&(Set->lines[i].Data[Offset]), data, WORD_SIZE);
                Set->lines[i].Dirty = 1;
                time += L2_WRITE_TIME;
            }
            //Atualiza o LRU para o valor qu não foi "hit"
            if(i == 0)
                Set->LRU = 1;
            else
                Set->LRU = 0;
            return;
        }
    }   

    // Cache Miss: se a linha não for válida ou o Tag não coincidir
    // Carrega o bloco da DRAM
    accessDRAM(MemAddress, TempBlock, MODE_READ);
        
    // Se a linha estiver suja, escreve o bloco antigo de volta na DRAM
    if (Set->lines[Set->LRU].Valid && Set->lines[Set->LRU].Dirty) {
        accessDRAM(MemAddress, Set->lines[Set->LRU].Data, MODE_WRITE);
    }

    // Substitui o bloco na linha de cache no LRU 
    memcpy(Set->lines[Set->LRU].Data, TempBlock, BLOCK_SIZE);
    Set->lines[Set->LRU].Valid = 1;
    Set->lines[Set->LRU].Tag = Tag;
    Set->lines[Set->LRU].Dirty = 0; // Novo bloco não é sujo inicialmente
    // Leitura
    if (mode == MODE_READ) {

        memcpy(data, &(Set->lines[Set->LRU].Data[Offset]), WORD_SIZE);
        time += L2_READ_TIME;
    }

    // Escrita
    if (mode == MODE_WRITE) {

        memcpy(&(Set->lines[Set->LRU].Data[Offset]), data, WORD_SIZE);
        Set->lines[Set->LRU].Dirty = 1;
        time += L2_WRITE_TIME;
    }
    if(Set->LRU == 0)
        Set->LRU = 1;
    else
        Set->LRU = 0;
    return;
    //Atualiza o LRU para o valor qu não foi atualizado
}

// Função principal para ler e escrever, considerando a hierarquia de cache L1 e L2
void read(uint32_t address, uint8_t *data) {
    // Acessar L1
    accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
    // Acessar L1
    accessL1(address, data, MODE_WRITE);
}
