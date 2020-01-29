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
    a = p->inct + (i - p->ops[i].a);
    b = p->inct + (i - p->ops[i].b);
    c = p->inct + (i - p->ops[i].c);

    if(unop){
      depmap[i+p->inct] = depmap[a];
    }else if(binop){
      depmap[i+p->inct] = depmap[a] | depmap[b];
    }else if(trinop){
      depmap[i+p->inct] = depmap[a] | depmap[b] | depmap[c];
    }
  }

  for(int i = 0; i < p->exct; i++){
    ret |= (depmap[p->exs[i]+p->inct] << (8 * i));
  }

  return ret;
}


/*
  Ideally, there would be different tables for different platforms, but that
  isn't much of a factor for this prototype.
*/
OPCODETABLE makeOpcodeTable(){
  OPCODETABLE ret;
  ret.isDefined[0] = 0b0000000000000000000000000011111111111111111111111111111111111110;
  ret.isDefined[1] = 0;
  ret.isDefined[2] = 0;
  ret.isDefined[3] = 0;

  ret.isUnop   [0] = 0b0000000000000000000000000011100000000000001110000100011000000000;
  ret.isUnop   [1] = 0;
  ret.isUnop   [2] = 0;
  ret.isUnop   [3] = 0;

  ret.isBinop  [0] = 0b0000000000000000000000000000011111111111110001111011100111100110;
  ret.isBinop  [1] = 0;
  ret.isBinop  [2] = 0;
  ret.isBinop  [3] = 0;

  ret.isTrinop [0] = 0b0000000000000000000000000000000000000000000000000000000000011000;
  ret.isTrinop [1] = 0;
  ret.isTrinop [2] = 0;
  ret.isTrinop [3] = 0;

  uint64_t cost_1 =  0b0000000000000000000000000011111111111111000111111111100000011110;
  uint64_t cost_3 =  0b0000000000000000000000000000000000000000111000000000000001100000;
  uint64_t cost_22=  0b0000000000000000000000000000000000000000000000000000000110000000;
  for(int i = 0; i < 256; i++){
    if(i < 64){
      if      (cost_1  & (1l << i)){
        ret.cost[i] =  1;
      }else if(cost_3  & (1l << i)){
        ret.cost[i] =  3;
      }else if(cost_22 & (1l << i)){
        ret.cost[i] = 22;
      }else{
        ret.cost[i] = 0;
      }
    }else{
      ret.cost[i] = 0;
    }
    ret.reqOpc[i] = 0;
  }
  ret.reqOpc[IMOD] = IDIV;
  ret.reqOpc[UMOD] = UDIV;

  return ret;
}


OPCODE newUnop(OPCODES op, int a){
  OPCODE ret;
  ret.op = op;
  ret.a  = a;
  return ret;
}

OPCODE newBinop(OPCODES op, int a, int b){
  OPCODE ret;
  ret.op = op;
  ret.a  = a;
  ret.b  = b;
  return ret;
}

OPCODE newTrinop(OPCODES op, int a, int b, int c){
  OPCODE ret;
  ret.op = op;
  ret.a  = a;
  ret.b  = b;
  ret.c  = c;
  return ret;
}





int main(){

  PROGRAM p;
  OPCODETABLE tab = makeOpcodeTable();

  p.ops[0] = newUnop (PCT , 4);
  p.ops[1] = newBinop(ADD , 4, 5);
  p.ops[2] = newBinop(ADD , 1, 4);
  p.ops[3] = newBinop(IMUL, 4, 6);
  p.inct   = 4;
  p.exct   = 4;
  p.exs[0] = 0;
  p.exs[1] = 1;
  p.exs[2] = 2;
  p.exs[3] = 3;
  p.size   = 4;

  uint64_t depmap = getDependencyMap(&tab, &p);
  for(int i = 0; i < 64; i++){
    printf((depmap & (1l << i))? "#" : " ");
  }
  printf("\n");

}
