#ifndef __UTIL_HEADER__
#define __UTIL_HEADER__


#include "stdint.h"





typedef struct{
  uint64_t bits[4];
}SET256;



int  nextSet64 (uint64_t, int);
int  nextSet256(SET256,   int);


void printU8 (uint8_t);
void printU16(uint16_t);
void printU32(uint32_t);
void printU64(uint64_t);


uint64_t rng();



#endif
