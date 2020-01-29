#ifndef __OPCODES_HEADER__
#define __OPCODES_HEADER__


#include "stdint.h"



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








uint64_t getDependencyMap(OPCODETABLE*, PROGRAM*);

OPCODETABLE makeOpcodeTable();

OPCODE newUnop(OPCODES, int);

OPCODE newBinop(OPCODES, int, int);

OPCODE newTrinop(OPCODES, int, int, int);








#endif
