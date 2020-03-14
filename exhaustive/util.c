#include "util.h"
#include "stdint.h"
#include "stdio.h"





static uint64_t rstate = 135181708917589;

// Some crazy rng function
uint64_t rng(){
  uint64_t a = rstate, b = rstate;
  {
    int rot = a >> 58;
    a = (a * 153178371081) + 135918077187;
    a ^= (a & 1)? 153718071081 : 13571089718090;
    a = (a >> rot) | (a << (64 - rot));
  }

  {
    int rot = b % 64;
    b = (b * 1531351311) + 13591574347187;
    b ^= (b & 2)? 8256413087 : 1357234728098;
    b = (b << rot) | (b >> (64 - rot));
  }
  rstate = a ^ b;
  return rstate;
}






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
