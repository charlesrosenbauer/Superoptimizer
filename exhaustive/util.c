#include "util.h"
#include "stdint.h"
#include "stdio.h"





void printU8 (uint8_t x){
  for(int i = 0; i < 8; i++){
    printf((x & (1l << i))? "#" : " ");
  }
  printf("\n");
}

void printU64(uint8_t x){
  for(int i = 0; i < 64; i++){
    printf((x & (1l << i))? "#" : " ");
  }
  printf("\n");
}
