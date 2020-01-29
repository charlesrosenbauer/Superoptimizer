#include "stdint.h"
#include "stdio.h"








typedef enum {
  ADD  =  1,
  SUB  =  2,
  ADC  =  3,
  SBC  =  4,
  IMUL =  5,
  UMUL =  6,
  IDIV =  7,
  UDIV =  8,
  IMOD =  9,
  UMOD = 10,
  XOR  = 11,
  AND  = 12,
  OR   = 13,
  NOT  = 14,
  SHL  = 15,
  SHR  = 16,
  RTL  = 17,
  RTR  = 18,
  PCT  = 19,
  CTZ  = 20,
  CLZ  = 21,
  MATCH= 22,
  DSJT = 23,
  OVLP = 24,
  ILS  = 25,
  ULS  = 26,
  ILSE = 27,
  ULSE = 28,
  IGT  = 29,
  UGT  = 30,
  IGTE = 31,
  UGTE = 32,
  EQ   = 33,
  NEQ  = 34,
  OF   = 35,
  EZ   = 36,
  NZ   = 37
}OPCODES;



typedef struct{
  uint64_t   isDefined[4];
  uint64_t   isUnop   [4];
  uint64_t   isBinop  [4];
  uint64_t   isTrinop [4];

  uint8_t    cost   [256];
  uint8_t    reqOpc [256];
}OPCODETABLE;



typedef struct{
  uint8_t  op, a, b, c;
}OPCODE;

typedef struct{
  OPCODE   ops[32];
  int      size, inct, exct;
  int      exs[8];
}PROGRAM;



uint64_t getDependencyMap(OPCODETABLE* table, PROGRAM* p){
  uint64_t ret = 0;

  uint8_t depmap[40];
  depmap[0] =   1;
  depmap[1] =   2;
  depmap[2] =   4;
  depmap[3] =   8;
  depmap[4] =  16;
  depmap[5] =  32;
  depmap[6] =  64;
  depmap[7] = 128;
  for(int i = 0; i < p->size; i++){
    uint64_t mask = 1l << (p->ops[i].op % 64);
    int      ix   =        p->ops[i].op / 64 ;

    int def    = (table->isDefined[ix] & mask) != 0;
    int unop   = (table->isUnop   [ix] & mask) != 0;
    int binop  = (table->isBinop  [ix] & mask) != 0;
    int trinop = (table->isTrinop [ix] & mask) != 0;

    if(!def){
      return 0;
    }

    int a, b, c;
    a = 8 + (i - p->ops[i].a);
    b = 8 + (i - p->ops[i].b);
    c = 8 + (i - p->ops[i].c);

    if(unop){
      depmap[i+8] = depmap[a];
    }else if(binop){
      depmap[i+8] = depmap[a] | depmap[b];
    }else if(trinop){
      depmap[i+8] = depmap[a] | depmap[b] | depmap[c];
    }
  }

  for(int i = 0; i < p->exct; i++){
    ret |= (depmap[p->exs[i]+8] << (8 * i));
  }

  return ret;
}





int main(){

}
