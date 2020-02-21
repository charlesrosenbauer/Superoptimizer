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
    a = p->inct + (i - (p->ops[i].a / 2));
    b = p->inct + (i - (p->ops[i].b / 2));
    c = p->inct + (i - (p->ops[i].c / 2));

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


typedef enum {
  IS_UNOP   =  1,
  IS_BINOP  =  2,
  IS_TRINOP =  4,
  IS_BIRET  =  8,
  IS_ORDERED= 16
}OPPROPFLAGS;

void newOpcode(OPCODETABLE* table, int opcode, char* name, int cost, OPPROPFLAGS flags){
  int wordix = opcode / 64;
  int bitix  = opcode % 64;

  table->cost[opcode] = cost;
  table->isDefined[wordix] |= (1l << bitix);
  table->isUnop   [wordix] |= (flags & IS_UNOP   )? (1l << bitix) : 0;
  table->isBinop  [wordix] |= (flags & IS_BINOP  )? (1l << bitix) : 0;
  table->isTrinop [wordix] |= (flags & IS_TRINOP )? (1l << bitix) : 0;
  table->isOrdered[wordix] |= (flags & IS_ORDERED)? (1l << bitix) : 0;

  if(opcode > table->maxOpIx){
    table->maxOpIx = opcode;
  }
  if(opcode < table->minOpIx){
    table->minOpIx = opcode;
  }
}


/*
  Ideally, there would be different tables for different platforms, but that
  isn't much of a factor for this prototype.
*/
OPCODETABLE makeOpcodeTable(){
  OPCODETABLE ret;
  ret.maxOpIx =   0;
  ret.minOpIx = 255;

  for(int i = 0; i < 256; i++){
    ret.cost  [i] = 0;
    ret.opname[i] = "";
  }
  for(int i = 0; i < 4; i++){
    ret.isDefined[i] = 0;
    ret.isUnop   [i] = 0;
    ret.isBinop  [i] = 0;
    ret.isTrinop [i] = 0;
    ret.isOrdered[i] = 0;
  }

  newOpcode(&ret, ADDI , "addi" ,  1, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, SUBI , "subi" ,  1, IS_BINOP);
  newOpcode(&ret, MULI , "muli" ,  3, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, MULU , "mulu" ,  3, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, DIVI , "divi" , 22, IS_BINOP | IS_BIRET  );
  newOpcode(&ret, DIVU , "divu" , 22, IS_BINOP | IS_BIRET  );
  newOpcode(&ret, AND  , "and"  ,  1, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, OR   , "or"   ,  1, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, XOR  , "xor"  ,  1, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, NOT  , "not"  ,  1, IS_UNOP );
  newOpcode(&ret, SHL  , "shl"  ,  1, IS_BINOP);
  newOpcode(&ret, SHR  , "shr"  ,  1, IS_BINOP);
  newOpcode(&ret, RTL  , "rtl"  ,  1, IS_BINOP);
  newOpcode(&ret, RTR  , "rtr"  ,  1, IS_BINOP);
  newOpcode(&ret, MAXI , "maxi" ,  3, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, MINI , "mini" ,  3, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, MAXU , "maxu" ,  3, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, MINU , "minu" ,  3, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, ILS  , "ils"  ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, ILSE , "ilse" ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, IGT  , "igt"  ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, IGTE , "igte" ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, EQ   , "eq"   ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, NEQ  , "neq"  ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, ULS  , "uls"  ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, ULSE , "ulse" ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, UGT  , "ugt"  ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, UGTE , "ugte" ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, NZ   , "nz"   ,  2, IS_UNOP);
  newOpcode(&ret, PCT  , "pct"  ,  1, IS_UNOP);
  newOpcode(&ret, CTZ  , "ctz"  ,  1, IS_UNOP);
  newOpcode(&ret, CLZ  , "clz"  ,  1, IS_UNOP);
  newOpcode(&ret, MTCH , "mtch" ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, SBST , "sbst" ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, SPST , "spst" ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, DSJT , "dsjt" ,  2, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, NCMP , "ncmp" ,  6, IS_BINOP | IS_ORDERED);
  newOpcode(&ret, SXOR , "sxor" , 18, IS_UNOP);
  newOpcode(&ret, SAND , "sand" , 18, IS_UNOP);
  newOpcode(&ret, SOR  , "sor"  , 18, IS_UNOP);
  newOpcode(&ret, SXNOR, "sxnor", 24, IS_UNOP);
  newOpcode(&ret, SNAND, "snand", 24, IS_UNOP);
  newOpcode(&ret, SNOR , "snor" , 24, IS_UNOP);
  newOpcode(&ret, INC  , "inc"  ,  1, IS_UNOP);
  newOpcode(&ret, DEC  , "dec"  ,  1, IS_UNOP);
  newOpcode(&ret, ADDO , "addo" ,  2, IS_BINOP | IS_ORDERED | IS_BIRET);
  newOpcode(&ret, SUBO , "subo" ,  2, IS_BINOP | IS_BIRET);
  newOpcode(&ret, UMULO, "umulo",  4, IS_BINOP | IS_ORDERED | IS_BIRET);
  newOpcode(&ret, IMULO, "imulo",  4, IS_BINOP | IS_ORDERED | IS_BIRET);
  newOpcode(&ret, ADC  , "adc"  ,  1, IS_TRINOP);
  newOpcode(&ret, SBB  , "sbb"  ,  1, IS_TRINOP);
  newOpcode(&ret, NEG  , "neg"  ,  1, IS_UNOP);
  newOpcode(&ret, ABS  , "abs"  ,  2, IS_UNOP);
  newOpcode(&ret, LEA1 , "lea1" ,  2, IS_TRINOP);
  newOpcode(&ret, LEA2 , "lea2" ,  2, IS_TRINOP);
  newOpcode(&ret, LEA4 , "lea4" ,  2, IS_TRINOP);
  newOpcode(&ret, LEA8 , "lea8" ,  2, IS_TRINOP);
  newOpcode(&ret, LEA1S, "lea1s",  2, IS_TRINOP);
  newOpcode(&ret, LEA2S, "lea2s",  2, IS_TRINOP);
  newOpcode(&ret, LEA4S, "lea4s",  2, IS_TRINOP);
  newOpcode(&ret, LEA8S, "lea8s",  2, IS_TRINOP);
  newOpcode(&ret, CMOV , "cmov" ,  2, IS_TRINOP);


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



int isUnop   (OPCODETABLE* tab, OPCODES op){
  int wordix = op / 64;
  int bitix  = op % 64;
  return (tab->isUnop   [wordix] & (1l << bitix)) != 0;
}


int isBinop  (OPCODETABLE* tab, OPCODES op){
  int wordix = op / 64;
  int bitix  = op % 64;
  return (tab->isBinop  [wordix] & (1l << bitix)) != 0;
}


int isTrinop (OPCODETABLE* tab, OPCODES op){
  int wordix = op / 64;
  int bitix  = op % 64;
  return (tab->isTrinop [wordix] & (1l << bitix)) != 0;
}


int isBiret  (OPCODETABLE* tab, OPCODES op){
  int wordix = op / 64;
  int bitix  = op % 64;
  return (tab->isBiret  [wordix] & (1l << bitix)) != 0;
}


int isOrdered(OPCODETABLE* tab, OPCODES op){
  int wordix = op / 64;
  int bitix  = op % 64;
  return (tab->isOrdered[wordix] & (1l << bitix)) != 0;
}
