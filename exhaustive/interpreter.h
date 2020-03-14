#ifndef __INTERPRETER_HEADER__
#define __INTERPRETER_HEADER__


#include "opcodes.h"


typedef union{
  int64_t  I64;
  uint64_t U64;
}VAL;


typedef struct{
  VAL       *aRets, *bRets;
  uint64_t  *passes;
}TESTROW;




typedef struct{
  TESTROW    tests[48];

  int depth, size, inct, exct;
}TESTCASE;



void step(OPCODETABLE*, TESTCASE*, PROGRAM*, int);
TESTCASE makeTests(OPCODETABLE*, PROGRAM*, int);




#endif
