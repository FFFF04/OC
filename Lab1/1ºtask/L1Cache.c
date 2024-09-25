#include "L1Cache.h"

uint8_t DRAM[DRAM_SIZE];
uint32_t time;
Cache SimpleCache;

/**************** Time Manipulation ***************/
void resetTime() { time = 0; }

uint32_t getTime() { return time; }

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t address, uint8_t *data, uint32_t mode) {

  if (address >= DRAM_SIZE - WORD_SIZE + 1)
    exit(-1);

  if (mode == MODE_READ) {
    memcpy(data, &(DRAM[address]), BLOCK_SIZE);
    time += DRAM_READ_TIME;
  }

  if (mode == MODE_WRITE) {
    memcpy(&(DRAM[address]), data, BLOCK_SIZE);
    time += DRAM_WRITE_TIME;
  }
}

/*********************** L1 cache *************************/

void initCache() {SimpleCache.init = 0;}

void accessL1(uint32_t address, uint8_t *data, uint32_t mode) {
    uint32_t index, Tag, MemAddress, Offset;
    uint8_t TempBlock[BLOCK_SIZE];

    /* Init cache */
    if (SimpleCache.init == 0) {
        for (int i = 0; i < NUM_LINES_L1; i++) {
            SimpleCache.lines[i].Valid = 0;
            SimpleCache.lines[i].Dirty = 0;
            SimpleCache.lines[i].Tag = 0;

            for (int j = 0; j < BLOCK_SIZE; j+=WORD_SIZE)
              SimpleCache.lines[i].Data[j] = 0;
        }
        SimpleCache.init = 1;
    }
    /* Calcula o índice e a tag */

    Offset = address & 0x3F;
    index = (address >> 6) & 0xFF;
    Tag = address >> 14;

    // Address - offset porque queremos ir para o inicio do bloco
    MemAddress = address - Offset;

    CacheLine *Line = &SimpleCache.lines[index];

    /* Acesso à Cache */
    if (!Line->Valid || Line->Tag != Tag) { // Cache miss
        // Busca o bloco na DRAM
        accessDRAM(MemAddress, TempBlock, MODE_READ);
        
        // Se a linha estiver suja, escreve o bloco antigo de volta na DRAM
        if (Line->Valid && Line->Dirty)
            accessDRAM(MemAddress, Line->Data, MODE_WRITE);

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

void read(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_READ);
}

void write(uint32_t address, uint8_t *data) {
  accessL1(address, data, MODE_WRITE);
}
