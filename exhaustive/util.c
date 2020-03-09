#include "util.h"
#include "stdint.h"
#include "stdio.h"






int nextSet64 (uint64_t bits, int ix){
  bits >>= (ix + 1);
  return (bits != 0)? __builtin_ctz(bits) + ix : -1;
}

int nextSet256(SET256   bits, int ix){
  int i = ix / 64;
  int j = ix % 64;

  bits.bits[i] >>= (j + 1);
  for(int k = i; k < 4; k++){
    if(bits.bits[k] != 0) return __builtin_ctz(bits.bits[k]) + ix;
  }
  return -1;
}



void printU8 (uint8_t x){
  for(int i = 0; i < 8; i++){
    printf((x & (1l << i))? "#" : " ");
  }
  printf("\n");
}

void printU16(uint16_t x){
  for(int i = 0; i < 16; i++){
    printf((x & (1l << i))? "#" : " ");
  }
  printf("\n");
}

void printU32(uint32_t x){
  for(int i = 0; i < 32; i++){
    printf((x & (1l << i))? "#" : " ");
  }
  printf("\n");
}

void printU64(uint64_t x){
  for(int i = 0; i < 64; i++){
    printf((x & (1l << i))? "#" : " ");
  }
  printf("\n");
}
