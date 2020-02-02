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
        __int128 ax, bx;
        ax = a[i].val.I64;
        bx = b[i].val.I64;
        ax += bx;
        q[i].val.I64  = (int64_t) ax;
        q[i].sidedata = (int64_t)(ax >> 64) != 0;
        q[i].metadata = 1;
      }
    }break;

    case SUB : {
      for(int i = 0; i < tests->tests; i++){
        __int128 ax, bx;
        ax = a[i].val.I64;
        bx = b[i].val.I64;
        ax -= bx;
        q[i].val.I64  = (int64_t) ax;
        q[i].sidedata = (int64_t)(ax >> 64) != 0;
        q[i].metadata = 1;
      }
    }break;

    case ADC : {
      // TODO: Needs to account for carry bit
      for(int i = 0; i < tests->tests; i++){
        __int128 ax, bx;
        ax = a[i].val.I64;
        bx = b[i].val.I64;
        ax += bx + c[i].sidedata;
        q[i].val.I64  = (int64_t)ax;
        q[i].sidedata = (int64_t)(ax >> 64) != 0;
        q[i].metadata = 1;
      }
    }break;

    case SBC : {
      // TODO: Needs to account for carry bit
      for(int i = 0; i < tests->tests; i++){
        __int128 ax, bx;
        ax = a[i].val.I64;
        bx = b[i].val.I64;
        ax = ax - bx - c[i].sidedata;
        q[i].val.I64  = (int64_t) ax;
        q[i].sidedata = (int64_t)(ax >> 64) != 0;
        q[i].metadata = 1;
      }
    }break;

    case IMUL: {
      for(int i = 0; i < tests->tests; i++){
        __int128 ax, bx;
        ax = a[i].val.I64;
        bx = b[i].val.I64;
        ax *= bx;
        q[i].val.I64  = (int64_t) ax;
        q[i].sidedata = (int64_t)(ax >> 65) != 0;   // Probably not quite right
        q[i].metadata = 1;
      }
    }break;

    case UMUL: {
      for(int i = 0; i < tests->tests; i++){
        __int128 ax, bx;
        uint64_t am, bm;
        ax = am = a[i].val.U64;
        bx = bm = b[i].val.U64;
        am *= bm;
        ax *= bx;
        q[i].val.U64  = (uint64_t) am;
        q[i].sidedata = (int64_t)(ax >> 65) != 0;   // Probably not quite right
        q[i].metadata = 1;
      }
    }break;

    case IDIV: {
      for(int i = 0; i < tests->tests; i++){
        if(b[i].val.I64 == 0){
          q[i].val.I64  = 0;
          q[i].metadata = 0;
        }else{
          q[i].val.I64  = a[i].val.I64 / b[i].val.I64;
          q[i].metadata = 1;
        }
      }
    }break;

    case UDIV: {
      for(int i = 0; i < tests->tests; i++){
        if(b[i].val.U64 == 0){
          q[i].val.U64  = 0;
          q[i].metadata = 0;
        }else{
          q[i].val.U64  = a[i].val.U64 / b[i].val.U64;
          q[i].metadata = 1;
        }
      }
    }break;

    case XOR : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 ^ b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case AND : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 & b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case OR  : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 | b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case NOT : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = ~a[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case SHL : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 << b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case SHR : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 >> b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case RTL : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = (a[i].val.U64 << b[i].val.U64) | (a[i].val.U64 >> (64 - b[i].val.U64));
        q[i].metadata = 1;
      }
    }break;

    case RTR : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = (a[i].val.U64 >> b[i].val.U64) | (a[i].val.U64 << (64 - b[i].val.U64));
        q[i].metadata = 1;
      }
    }break;

    case PCT : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = __builtin_popcountl(a[i].val.U64);
        q[i].metadata = 1;
      }
    }break;

    case CTZ : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = __builtin_ctz(a[i].val.U64);
        q[i].metadata = 1;
      }
    }break;

    case CLZ : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = __builtin_clz(a[i].val.U64);
        q[i].metadata = 1;
      }
    }break;

    case ILS : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.I64 < b[i].val.I64;
        q[i].metadata = 1;
      }
    }break;

    case ULS : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 < b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case ILSE : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.I64 <= b[i].val.I64;
        q[i].metadata = 1;
      }
    }break;

    case ULSE : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 <= b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case IGT : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.I64 > b[i].val.I64;
        q[i].metadata = 1;
      }
    }break;

    case UGT : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 > b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;

    case IGTE : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.I64 >= b[i].val.I64;
        q[i].metadata = 1;
      }
    }break;

    case UGTE : {
      for(int i = 0; i < tests->tests; i++){
        q[i].val.U64 = a[i].val.U64 >= b[i].val.U64;
        q[i].metadata = 1;
      }
    }break;
  }


  return 0;
}
