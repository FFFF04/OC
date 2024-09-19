#ifndef CACHEL2_H
#define CACHEL2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "Cache.h"

#define NUM_LINES_L1 256

#define NUM_LINES_L2 512

void resetTime();

uint32_t getTime();

/****************  RAM memory (byte addressable) ***************/
void accessDRAM(uint32_t, uint8_t *, uint32_t);

/*********************** Cache *************************/

void initCache();
void accessL1(uint32_t, uint8_t *, uint32_t);
void accessL2(uint32_t, uint8_t *,uint32_t );

// Estrutura da linha da cache
typedef struct CacheLine {
    uint8_t Valid;
    uint8_t Dirty;
    uint32_t Tag;
    uint8_t Data[BLOCK_SIZE];
} CacheLine;

// Estrutura das caches L1 e L2
typedef struct CacheL1 {
    uint32_t init;
    CacheLine lines[NUM_LINES_L1]; // Cache L1 com 256 linhas
} CacheL1;

typedef struct CacheL2 {
    uint32_t init;
    CacheLine lines[NUM_LINES_L2]; // Cache L2 com 512 linhas
} CacheL2;

/*********************** Interfaces *************************/

void read(uint32_t, uint8_t *);

void write(uint32_t, uint8_t *);

#endif