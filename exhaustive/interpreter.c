#include "interpreter.h"
#include "opcodes.h"
#include "stdint.h"









int step(OPCODETABLE* table, TESTCASE* tests, PROGRAM* p, int ix){

  REG *a, *b, *c, *q;
  int     op = p->ops[ix].op;
  int     tableslice = tests->tests;
  q = &tests->data[tableslice * ix];
  if((op < 0) || (op >= p->size)){
    return -1;
  }

  if((op >= 64) || (table->isDefined[0] & (1l << op))) {
    return -1;
  }else if((1l << op) & table->isUnop  [0]){
    a = &tests->data[tableslice * (ix - p->ops[ix].a)];
  }else if((1l << op) & table->isBinop [0]){
    a = &tests->data[tableslice * (ix - p->ops[ix].a)];
    b = &tests->data[tableslice * (ix - p->ops[ix].b)];
  }else if((1l << op) & table->isTrinop[0]){
    a = &tests->data[tableslice * (ix - p->ops[ix].a)];
    b = &tests->data[tableslice * (ix - p->ops[ix].b)];
    c = &tests->data[tableslice * (ix - p->ops[ix].c)];
  }else{
    return -1;
  }

  switch(op){
    case ADD : {
      for(int i = 0; i < tests->tests; i++){
        q[i].I64 = a[i].I64 + b[i].I64;
      }
    }break;

    case SUB : {
      for(int i = 0; i < tests->tests; i++){
        q[i].I64 = a[i].I64 - b[i].I64;
      }
    }break;

    case ADC : {
      // TODO: Needs to account for carry bit
      for(int i = 0; i < tests->tests; i++){
        q[i].I64 = a[i].I64 + b[i].I64;
      }
    }break;

    case SBC : {
      // TODO: Needs to account for carry bit
      for(int i = 0; i < tests->tests; i++){
        q[i].I64 = a[i].I64 + b[i].I64;
      }
    }break;

    case IMUL: {
      for(int i = 0; i < tests->tests; i++){
        q[i].I64 = a[i].I64 * b[i].I64;
      }
    }break;

    case UMUL: {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 * b[i].U64;
      }
    }break;

    case IDIV: {
      // TODO: Needs to account for zeroes
      for(int i = 0; i < tests->tests; i++){
        q[i].I64 = a[i].I64 / b[i].I64;
      }
    }break;

    case UDIV: {
      // TODO: Needs to account for zeroes
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 / b[i].U64;
      }
    }break;

    case XOR : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 ^ b[i].U64;
      }
    }break;

    case AND : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 & b[i].U64;
      }
    }break;

    case OR  : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 | b[i].U64;
      }
    }break;

    case NOT : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = ~a[i].U64;
      }
    }break;

    case SHL : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 << b[i].U64;
      }
    }break;

    case SHR : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 >> b[i].U64;
      }
    }break;

    case RTL : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = (a[i].U64 << b[i].U64) | (a[i].U64 >> (64 - b[i].U64));
      }
    }break;

    case RTR : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = (a[i].U64 >> b[i].U64) | (a[i].U64 << (64 - b[i].U64));
      }
    }break;

    case PCT : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = __builtin_popcountl(a[i].U64);
      }
    }break;

    case CTZ : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = __builtin_ctz(a[i].U64);
      }
    }break;

    case CLZ : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = __builtin_clz(a[i].U64);
      }
    }break;

    case ILS : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].I64 < b[i].I64;
      }
    }break;

    case ULS : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 < b[i].U64;
      }
    }break;

    case ILSE : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].I64 <= b[i].I64;
      }
    }break;

    case ULSE : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 <= b[i].U64;
      }
    }break;

    case IGT : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].I64 > b[i].I64;
      }
    }break;

    case UGT : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 > b[i].U64;
      }
    }break;

    case IGTE : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].I64 >= b[i].I64;
      }
    }break;

    case UGTE : {
      for(int i = 0; i < tests->tests; i++){
        q[i].U64 = a[i].U64 >= b[i].U64;
      }
    }break;
  }


  return 0;
}
