#include "interpreter.h"
#include "opcodes.h"
#include "stdint.h"
#include "stdlib.h"







void getParRows(OPCODETABLE* table, TESTCASE* tests, PROGRAM* p, int ix, VAL** a, VAL** b, VAL** c, TESTROW** ret){
  *a   = NULL;
  *b   = NULL;
  *c   = NULL;
  *ret = NULL;
  int opcode = p->ops[ix].op;
  int aix = p->ops[ix].a;
  int bix = p->ops[ix].b;
  int cix = p->ops[ix].c;
  int ares= aix % 2;
  int bres= bix % 2;
  int cres= cix % 2;
  aix = 4 + (ix - (aix / 2));
  bix = 4 + (ix - (bix / 2));
  cix = 4 + (ix - (cix / 2));

  if(      isUnop  (table, opcode)){
    *a = (ares)? (tests->tests[aix].aRets) : (tests->tests[aix].bRets);
  }else if(isBinop (table, opcode)){
    *a = (ares)? (tests->tests[aix].aRets) : (tests->tests[aix].bRets);
    *b = (bres)? (tests->tests[bix].aRets) : (tests->tests[bix].bRets);
  }else if(isTrinop(table, opcode)){
    *a = (ares)? (tests->tests[aix].aRets) : (tests->tests[aix].bRets);
    *b = (bres)? (tests->tests[bix].aRets) : (tests->tests[bix].bRets);
    *c = (cres)? (tests->tests[cix].aRets) : (tests->tests[cix].bRets);
  }

  *ret = &tests->tests[ix + 4];
}



void step(OPCODETABLE* table, TESTCASE* tests, PROGRAM* p, int ix){

  VAL *a, *b, *c;
  TESTROW* ret;
  int opcode = p->ops[ix].op;
  getParRows(table, tests, p, ix, &a, &b, &c, &ret);
  int testct = tests->size;

  switch(opcode){
    case ADDI : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 + b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case SUBI : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 - b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case MULI : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 * b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case MULU : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 * b[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case DIVI : {
      // Cases need to be added to check for divide-by-zero errors
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 / b[i].I64;
        ret->bRets[i].I64 = a[i].I64 % b[i].I64;
      }
    }break;

    case DIVU : {
      // Cases need to be added to check for divide-by-zero errors
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 / b[i].U64;
        ret->bRets[i].U64 = a[i].U64 % b[i].U64;
      }
    }break;

    case AND : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 & b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case OR : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 | b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case XOR : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 ^ b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case NOT : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = ~a[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case SHL : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 >> b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case SHR : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 << b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case RTL : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = (a[i].I64 >> b[i].I64) | (a[i].I64 << (64 - b[i].I64));
        ret->bRets[i].I64 = 0;
      }
    }break;

    case RTR : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = (a[i].I64 << b[i].I64) | (a[i].I64 >> (64 - b[i].I64));
        ret->bRets[i].I64 = 0;
      }
    }break;
  }


}


int  test(OPCODETABLE* table, TESTCASE* tests, PROGRAM* p, TESTROW* constraints, int ix){

  VAL *a, *b, *c;
  TESTROW* ret;
  int opcode = p->ops[ix].op;
  getParRows(table, tests, p, ix, &a, &b, &c, &ret);

  switch(opcode){

  }

  return 1;
}
