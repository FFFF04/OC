#ifndef CACHEL1_H
#define CACHEL1_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Cache.h"

#define NUM_LINES_L1 256

void resetTime();

uint32_t getTime();

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t, uint8_t *, uint32_t);

/*********************** Cache *************************/

void initCache();
void accessL1(uint32_t, uint8_t *, uint32_t);

typedef struct CacheLine {
    uint8_t Valid;
    uint8_t Dirty;
    uint32_t Tag;
    uint8_t Data[BLOCK_SIZE]; // Armazena o bloco de dados
} CacheLine;

typedef struct Cache {
    uint32_t init;
    CacheLine lines[NUM_LINES_L1]; // Várias linhas de cache L1
} Cache;

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);

#endif