#ifndef __CORE_HEADER__
#define __CORE_HEADER__


#include "stdint.h"




typedef enum{
  UNUSED, SINGLE_USE, MULTI_USE
}RETTYPE;


typedef enum{
  OP_1_1, OP_2_1, OP_2_2, OP_3_1, OP_INPUT
}OPTYPE;


typedef struct{
  uint64_t depmap;
  uint64_t deps  [40];
  RETTYPE  retyps[80];
  OPTYPE   optyps[40];
}PROGRAMDATA;









#endif
