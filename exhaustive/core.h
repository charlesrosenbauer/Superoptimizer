#ifndef __CORE_HEADER__
#define __CORE_HEADER__


#include "stdint.h"
#include "opcodes.h"
#include "interpreter.h"




typedef enum{
  UNUSED = 0, SINGLE_USE = 1, MULTI_USE = 2
}RETTYPE;


typedef enum{
  OP_1_1 = 0, OP_2_1 = 1, OP_2_2 = 2, OP_3_1 = 3, OP_INPUT = 4
}OPTYPE;


typedef struct{
  uint64_t depmap;
  uint64_t deps  [40];
  RETTYPE  retyps[80];
  OPTYPE   optyps[40];
}PROGRAMDATA;




int superoptimize(OPCODETABLE*, PROGRAM*, PROGRAM*, int, TESTCASE*, int);




#endif
