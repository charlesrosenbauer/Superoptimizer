#include "util.h"
#include "stdint.h"
#include "stdio.h"





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
