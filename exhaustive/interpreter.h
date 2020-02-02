#ifndef __INTERPRETER_HEADER__
#define __INTERPRETER_HEADER__


#include "opcodes.h"


typedef union{
  int64_t  I64;
  uint64_t U64;
}VAL;


typedef struct{
  VAL      val;
  uint64_t sidedata;
  uint8_t  metadata;
}REG;




typedef struct{
  REG *data;

  int tests, depth;
}TESTCASE;









#endif
