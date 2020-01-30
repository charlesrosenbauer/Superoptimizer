#include "interpreter.h"
#include "opcodes.h"
#include "stdint.h"









int step(OPCODETABLE* table, TESTCASE* tests, PROGRAM* p, int ix){
  if((op < 0) || (op >= p->size)){
    return -1;
  }

  REG *a, *b, *c, *q;
  int     op = p->ops[ix].op;
  int     tableslice = table->tests;
  q = tests->data[tableslice * ix];
  if((op >= 64) || (table->isDefined[0] & (1l << op))) {
    return -1;
  }else if((1l << op) & table->isUnop  [0]){
    a = tests->data[tableslice * (ix - p->ops[ix].a)];
  }else if((1l << op) & table->isBinop [0]){
    a = tests->data[tableslice * (ix - p->ops[ix].a)];
    b = tests->data[tableslice * (ix - p->ops[ix].b)];
  }else if((1l << op) & table->isTrinop[0]){
    a = tests->data[tableslice * (ix - p->ops[ix].a)];
    b = tests->data[tableslice * (ix - p->ops[ix].b)];
    c = tests->data[tableslice * (ix - p->ops[ix].c)];
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
  }

}
