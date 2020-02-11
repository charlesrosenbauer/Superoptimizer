#ifndef __OPCODES_HEADER__
#define __OPCODES_HEADER__


#include "stdint.h"



typedef enum {
  NOOP  = 0x00,
	ADDI  = 0x01,    // 1c
	SUBI  = 0x02,    // 1c
	MULI  = 0x03,    // 3c
	MULU  = 0x04,    // 3c
	DIVI  = 0x05,    // 22c
	DIVU  = 0x06,    // 22c
	MODI  = 0x07,    // 22c
	MODU  = 0x08,    // 22c
	AND   = 0x09,    // 1c
	OR    = 0x0A,    // 1c
	XOR   = 0x0B,    // 1c
	NOT   = 0x0C,    // 1c
	SHL   = 0x0D,    // 1c
	SHR   = 0x0E,    // 1c
	RTL   = 0x0F,    // 1c
	RTR   = 0x10,    // 1c
	MAXI  = 0x11,    // 3c
	MINI  = 0x12,    // 3c
	MAXU  = 0x13,    // 3c
	MINU  = 0x14,    // 3c
	ILS   = 0x15,    // 2c
	ILSE  = 0x16,    // 2c
	IGT   = 0x17,    // 2c
	IGTE  = 0x18,    // 2c
	EQ    = 0x19,    // 2c
	NEQ   = 0x1A,    // 2c
	ULS   = 0x1B,    // 2c
	ULSE  = 0x1C,    // 2c
	UGT   = 0x1D,    // 2c
	UGTE  = 0x1E,    // 2c
	NZ    = 0x1F,    // 2c
	PCT   = 0x20,    // 1c
	CTZ   = 0x21,    // 1c
	CLZ   = 0x22,    // 1c
	MTCH  = 0x23,    // 2c
	SBST  = 0x24,    // 2c
	SPST  = 0x25,    // 2c
	DSJT  = 0x26,    // 2c
	NCMP  = 0x27,    // 6c
	SXOR  = 0x28,    // 18c
	SAND  = 0x29,    // 18c
	SOR   = 0x2A,    // 18c
	SXNOR = 0x2B,    // 24c
	SNAND = 0x2C,    // 24c
	SNOR  = 0x2D,    // 24c
	INC   = 0x2E,    // 1c
	DEC   = 0x2F,    // 1c
  ADDO  = 0x30,    // 2c
  SUBO  = 0x31,    // 2c
  UMULO = 0x32,    // 4c
  IMULO = 0x33,    // 4c
  ADC   = 0x34,    // 1c
  SBB   = 0x35,    // 1c
  NEG   = 0x36,    // 1c
  ABS   = 0x37,    // 2c
  LEA1  = 0x38,    // 2c
  LEA2  = 0x39,    // 2c
  LEA4  = 0x3A,    // 2c
  LEA8  = 0x3B,    // 2c
  LEA1S = 0x3C,    // 2c
  LEA2S = 0x3D,    // 2c
  LEA4S = 0x3E,    // 2c
  LEA8S = 0x3F,    // 2c
  CMOV  = 0x40     // 2c

}OPCODES;



typedef struct{
  uint64_t   isDefined[4];
  uint64_t   isUnop   [4];
  uint64_t   isBinop  [4];
  uint64_t   isTrinop [4];
  uint64_t   isBiret  [4];
  uint64_t   isOrdered[4];

  uint8_t    cost   [256];
  char*      opname [256];
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

int isUnop   (OPCODETABLE*, OPCODES);
int isBinop  (OPCODETABLE*, OPCODES);
int isTrinop (OPCODETABLE*, OPCODES);
int isBiret  (OPCODETABLE*, OPCODES);
int isOrdered(OPCODETABLE*, OPCODES);








#endif
