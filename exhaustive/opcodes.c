#include "opcodes.h"
#include "stdint.h"
#include "stdio.h"







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
