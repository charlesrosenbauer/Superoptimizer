#include "interpreter.h"
#include "opcodes.h"
#include "stdint.h"
#include "stdlib.h"
#include "util.h"
#include "stdio.h"



uint64_t sxor(uint64_t x){
  x ^= (x >>  1);
  x ^= (x >>  2);
  x ^= (x >>  4);
  x ^= (x >>  8);
  x ^= (x >> 16);
  x ^= (x >> 32);
  return x;
}

uint64_t sand(uint64_t x){
  x &= (x >>  1);
  x &= (x >>  2);
  x &= (x >>  4);
  x &= (x >>  8);
  x &= (x >> 16);
  x &= (x >> 32);
  return x;
}

uint64_t sor(uint64_t x){
  x |= (x >>  1);
  x |= (x >>  2);
  x |= (x >>  4);
  x |= (x >>  8);
  x |= (x >> 16);
  x |= (x >> 32);
  return x;
}

uint64_t sxnor(uint64_t x){
  x ^= ~(x >>  1);
  x ^= ~(x >>  2);
  x ^= ~(x >>  4);
  x ^= ~(x >>  8);
  x ^= ~(x >> 16);
  x ^= ~(x >> 32);
  return x;
}

uint64_t snand(uint64_t x){
  x &= ~(x >>  1);
  x &= ~(x >>  2);
  x &= ~(x >>  4);
  x &= ~(x >>  8);
  x &= ~(x >> 16);
  x &= ~(x >> 32);
  return x;
}

uint64_t snor(uint64_t x){
  x |= ~(x >>  1);
  x |= ~(x >>  2);
  x |= ~(x >>  4);
  x |= ~(x >>  8);
  x |= ~(x >> 16);
  x |= ~(x >> 32);
  return x;
}









void getParRows(OPCODETABLE* table, TESTCASE* tests, PROGRAM* p, int ix, VAL** a, VAL** b, VAL** c, uint64_t** pa, uint64_t** pb, uint64_t** pc, uint64_t** pr, TESTROW** ret){
  *a   = NULL;
  *b   = NULL;
  *c   = NULL;
  *pa  = NULL;
  *pb  = NULL;
  *pc  = NULL;
  *pr  = NULL;
  *ret = NULL;
  int opcode = p->ops[ix].op;
  int aix    = p->ops[ix].a;
  int bix    = p->ops[ix].b;
  int cix    = p->ops[ix].c;
  int ares   = aix % 2;
  int bres   = bix % 2;
  int cres   = cix % 2;
  aix = (aix / 2);
  bix = (bix / 2);
  cix = (cix / 2);

  if(      isUnop  (table, opcode)){
    *a  = (!ares)? (tests->tests[aix].aRets) : (tests->tests[aix].bRets);
    *pa = tests->tests[aix].passes;
  }else if(isBinop (table, opcode)){
    *a  = (!ares)? (tests->tests[aix].aRets) : (tests->tests[aix].bRets);
    *b  = (!bres)? (tests->tests[bix].aRets) : (tests->tests[bix].bRets);
    *pa = tests->tests[aix].passes;
    *pb = tests->tests[bix].passes;
  }else if(isTrinop(table, opcode)){
    *a  = (!ares)? (tests->tests[aix].aRets) : (tests->tests[aix].bRets);
    *b  = (!bres)? (tests->tests[bix].aRets) : (tests->tests[bix].bRets);
    *c  = (!cres)? (tests->tests[cix].aRets) : (tests->tests[cix].bRets);
    *pa = tests->tests[aix].passes;
    *pb = tests->tests[bix].passes;
    *pc = tests->tests[cix].passes;
  }

  *ret = &tests->tests[ix + 8];
  *pr  =  tests->tests[ix + 8].passes;
}



void step(OPCODETABLE* table, TESTCASE* tests, PROGRAM* p, int ix){

  VAL      *a, *b, *c;
  uint64_t *pa, *pb, *pc, *pr;
  TESTROW  *ret;
  int opcode = p->ops[ix].op;
  getParRows(table, tests, p, ix, &a, &b, &c, &pa, &pb, &pc, &pr, &ret);
  int testct = tests->size;
  int passct = (testct % 64)? (testct / 64) + 1 : (testct / 64);

  if      (isUnop  (table, opcode)){
    for(int i = 0; i < passct; i++){
      ret->passes[i] = pa[i];
      pr[i] = ret->passes[i];
    }
  }else if(isBinop (table, opcode)){
    for(int i = 0; i < passct; i++){
      ret->passes[i] = pa[i] & pb[i];
      pr[i] = ret->passes[i];
    }
  }else if(isTrinop(table, opcode)){
    for(int i = 0; i < passct; i++){
      ret->passes[i] = pa[i] & pb[i] & pc[i];
      pr[i] = ret->passes[i];
    }
  }


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
        __int128 x = a[i].I64;
        __int128 y = b[i].I64;
        x *= y;
        ret->aRets[i].I64 = x;
        ret->bRets[i].I64 = x >> 64;
      }
    }break;

    case MULU : {
      for(int i = 0; i < testct; i++){
        unsigned __int128 x = a[i].U64;
        unsigned __int128 y = b[i].U64;
        x *= y;
        ret->aRets[i].U64 = x;
        ret->bRets[i].U64 = x >> 64;
      }
    }break;

    case DIVI : {
      for(int i = 0; i < testct; i++){
        if(b[i].I64 != 0){
          ret->aRets[i].I64 = a[i].I64 / b[i].I64;
          ret->bRets[i].I64 = a[i].I64 % b[i].I64;
        }else{
          ret->aRets[i].I64 = 0;
          ret->bRets[i].I64 = 0;
          pr[i / 64] &= ~(1l << (i % 64));
        }
      }
    }break;

    case DIVU : {
      for(int i = 0; i < testct; i++){
        if(b[i].I64 != 0){
          ret->aRets[i].I64 = a[i].U64 / b[i].U64;
          ret->bRets[i].I64 = a[i].U64 % b[i].U64;
        }else{
          ret->aRets[i].U64 = 0;
          ret->bRets[i].U64 = 0;
          pr[i / 64] &= ~(1l << (i % 64));
        }
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

    case MAXI : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = (a[i].I64 > b[i].I64)? a[i].I64 : b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case MINI : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = (a[i].I64 < b[i].I64)? a[i].I64 : b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case MAXU : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = (a[i].U64 > b[i].U64)? a[i].U64 : b[i].U64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case MINU : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = (a[i].U64 < b[i].U64)? a[i].U64 : b[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case ILS  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 <  b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case ILSE : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 <= b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case IGT  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 >  b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case IGTE : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 >= b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case EQ  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 == b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case NEQ : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = a[i].I64 != b[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case ULS  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 <  b[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case ULSE : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 <= b[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case UGT  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 >  b[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case UGTE : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 >= b[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case PCT  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = __builtin_popcountl(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case CTZ  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = __builtin_ctzl(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case CLZ  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = __builtin_clzl(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case MTCH : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = __builtin_popcountl(a[i].U64 & b[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SBST : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = (a[i].U64 & b[i].U64) == a[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SPST : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = (a[i].U64 & b[i].U64) == b[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case DSJT : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = (a[i].U64 & b[i].U64) == 0;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case NCMP : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = ((a[i].U64 & b[i].U64) != a[i].U64) && ((a[i].U64 & b[i].U64) != b[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SXOR  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = sxor(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SAND  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = sand(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SOR  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = sor(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SXNOR  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = sxnor(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SNAND  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = snand(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SNOR  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = snor(a[i].U64);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case INC  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + 1;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case DEC  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 - 1;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case ADDO : {
      for(int i = 0; i < testct; i++){
        int64_t of;
        ret->aRets[i].I64 = __builtin_saddl_overflow(a[i].I64, b[i].I64, &of); //(a[i].U64 & b[i].U64) == 0;
        ret->bRets[i].I64 = of;
      }
    }break;

    case SUBO : {
      for(int i = 0; i < testct; i++){
        int64_t of;
        ret->aRets[i].I64 = __builtin_ssubl_overflow(a[i].I64, b[i].I64, &of); //(a[i].U64 & b[i].U64) == 0;
        ret->bRets[i].I64 = of;
      }
    }break;

    case UMULO : {
      for(int i = 0; i < testct; i++){
        uint64_t of;
        ret->aRets[i].U64 = __builtin_umull_overflow(a[i].U64, b[i].U64, &of); //(a[i].U64 & b[i].U64) == 0;
        ret->bRets[i].U64 = of;
      }
    }break;

    case IMULO : {
      for(int i = 0; i < testct; i++){
        int64_t of;
        ret->aRets[i].I64 = __builtin_smull_overflow(a[i].I64, b[i].I64, &of); //(a[i].U64 & b[i].U64) == 0;
        ret->bRets[i].I64 = of;
      }
    }break;

    case ADC  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + b[i].U64 + (c[i].U64 != 0);
        ret->bRets[i].U64 = 0;
      }
    }break;

    case SBB  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 - (b[i].U64 + (c[i].U64 != 0));
        ret->bRets[i].U64 = 0;
      }
    }break;

    case NEG  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = -a[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case ABS  : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].I64 = (a[i].I64 < 0)? -a[i].I64 : a[i].I64;
        ret->bRets[i].I64 = 0;
      }
    }break;

    case LEA1 : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + (b[i].U64 * 1) + c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case LEA2 : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + (b[i].U64 * 2) + c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case LEA4 : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + (b[i].U64 * 4) + c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case LEA8 : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + (b[i].U64 * 8) + c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case LEA1S : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + (b[i].U64 * 1) - c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case LEA2S : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + (b[i].U64 * 2) - c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case LEA4S : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + (b[i].U64 * 4) - c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case LEA8S : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = a[i].U64 + (b[i].U64 * 8) - c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;

    case CMOV : {
      for(int i = 0; i < testct; i++){
        ret->aRets[i].U64 = (a[i].U64)? b[i].U64 : c[i].U64;
        ret->bRets[i].U64 = 0;
      }
    }break;
  }
}




TESTCASE makeTests(OPCODETABLE* tab, PROGRAM* p, int testct){
  TESTCASE ret;
  ret.size = testct;
  int passct = (testct % 64)? ((testct / 64) + 1) : (testct / 64);
  for(int i = 0; i < 48; i++){
    ret.tests[i].aRets  = malloc(sizeof(VAL) * testct);
    ret.tests[i].bRets  = malloc(sizeof(VAL) * testct);
    ret.tests[i].passes = malloc(sizeof(uint64_t) * passct);
  }

  for(int i = 0; i < p->inct; i++){
    for(int j = 0; j < testct; j++) ret.tests[i].aRets [j].U64 = rng();
    for(int j = 0; j < passct; j++) ret.tests[i].passes[j] = 0xffffffffffffffff;
  }

  for(int i = 0; i < p->size; i++){
    printf("%i\n", i);
    step(tab, &ret, p, i);
  }

  for(int i = 0; i < p->exct; i++){
    if(!(p->exs[i] & 1)){
      for(int j = 0; j < testct; j++)
        ret.tests[i+40].aRets[j] = ret.tests[p->exs[i]/2].aRets[j];
    }else{
      for(int j = 0; j < testct; j++)
        ret.tests[i+40].aRets[j] = ret.tests[p->exs[i]/2].bRets[j];
    }
  }

  return ret;
}
