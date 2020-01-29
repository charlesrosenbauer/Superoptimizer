#include "stdint.h"
#include "stdio.h"


typedef struct{
  uint16_t ops[32];
  int      length;
}CODE;


uint64_t rstate = 15183071891781;
uint64_t rng(){
  rstate = (rstate * 513750891) + 1517981023;
  int rshift = rstate % 64;
  rstate = (rstate << rshift) | (rstate >> (64 - rshift));
  return rstate;
}


void conformOp(CODE* c, int opix){
  if(opix < c->length){
    uint16_t op = c->ops[opix];

  }
}


CODE deviate(CODE c){
  CODE ret;
  rng();
  switch(rstate % 4){
    case 0:{ // Insert
      if(c.length < 32){
        ret.length = c.length + 1;
        int j = 0;
        int insertIndex = (rstate / 4) % c.length;
        for(int i = 0; i < c.length; i++){
          if(i != insertIndex){
            ret.ops[i] = c.ops[j];
            j++;
          }else{
            ret.ops[i] = rstate >> 48;
          }
        }
      }
    }break;
    case 1:{ // Remove
      int j = 0;
      int insertIndex = (rstate / 4) % c.length;
      for(int i = 0; i < c.length; i++){
        if(i != insertIndex){
          ret.ops[i] = c.ops[j];
        }else{
          j++;
        }
        j++;
      }
    }break;
    case 2:{ // Change
      ret = c;
      int deltaIndex = (rstate / 4) % c.length;
      ret.ops[deltaIndex] = rstate >> 48;
    }break;
    case 3:{ // Swap parameters
      ret = c;
      int deltaIndex = (rstate / 4) % c.length;
      uint64_t op = ret.ops[deltaIndex];
      int a = (op     ) & 0x7;
      int b = (op >> 3) & 0x7;
      op = (op & 0xffc0) | (a << 3) | b;
    }break;
  }
  return ret;
}

typedef struct{
  uint8_t regs[8];
}REGS;


void eval(CODE c, REGS* r){


  /*
    OPS:
    00 ADD q a b
    01 SUB q a b
    02 MUL q a b
    03 DIV q a b
    04 MOD q a b
    05 AND q a b
    06 OR  q a b
    07 XOR q a b
    08 NOT q a
    09 SHL q a b
    0a SHR q a b
    0b RTL q a b
    0c RTR q a b
    0d LS  q a b
    0e LSE q a b
    0f GT  q a b
    10 GTE q a b
    11 EQ  q a b
    12 NEQ q a b
    13 CTZ q a
    14 CLZ q a
    15 PCT q a
    16 MCH q a b
    17 CST q imm
    18 MIN q a b
    19 MAX q a b
    1a SPS q a b
    1b SBS q a b
    1c DSJ q a b

  */

  uint8_t regs[8];
  for(int i = 0; i < c.length; i++){
    uint16_t op = c.ops[i];
    int q   = (op      ) & 0x7;
    int a   = (op >>  3) & 0x7;
    int b   = (op >>  6) & 0x7;
    int opc = (op >>  9);
    switch(opc){
      case 0x00 : r->regs[q] =  r->regs[a] +  r->regs[b];  break;
      case 0x01 : r->regs[q] =  r->regs[a] -  r->regs[b];  break;
      case 0x02 : r->regs[q] =  r->regs[a] *  r->regs[b];  break;
      case 0x03 : r->regs[q] = (r->regs[b] != 0)? r->regs[a] /  r->regs[b] : 0;  break;
      case 0x04 : r->regs[q] = (r->regs[b] != 0)? r->regs[a] %  r->regs[b] : 0;  break;
      case 0x05 : r->regs[q] =  r->regs[a] &  r->regs[b];  break;
      case 0x06 : r->regs[q] =  r->regs[a] |  r->regs[b];  break;
      case 0x07 : r->regs[q] =  r->regs[a] ^  r->regs[b];  break;
      case 0x08 : r->regs[q] = ~r->regs[a];            break;
      case 0x09 : r->regs[q] =  r->regs[a] << r->regs[b];  break;
      case 0x0A : r->regs[q] =  r->regs[a] >> r->regs[b];  break;
      case 0x0B : r->regs[q] = (r->regs[a] << r->regs[b]) | (r->regs[a] >> (64 - r->regs[b]));  break;
      case 0x0C : r->regs[q] = (r->regs[a] >> r->regs[b]) | (r->regs[a] << (64 - r->regs[b]));  break;
      case 0x0D : r->regs[q] =  r->regs[a] <  r->regs[b];  break;
      case 0x0E : r->regs[q] =  r->regs[a] <= r->regs[b];  break;
      case 0x0F : r->regs[q] =  r->regs[a] >  r->regs[b];  break;
      case 0x10 : r->regs[q] =  r->regs[a] >= r->regs[b];  break;
      case 0x11 : r->regs[q] =  r->regs[a] == r->regs[b];  break;
      case 0x12 : r->regs[q] =  r->regs[a] != r->regs[b];  break;
      case 0x13 : r->regs[q] =  __builtin_ctz(r->regs[a]);  break;
      case 0x14 : r->regs[q] =  __builtin_clz(r->regs[a]);  break;
      case 0x15 : r->regs[q] =  __builtin_popcount(r->regs[a]);  break;
      case 0x16 : r->regs[q] =  __builtin_popcount(r->regs[a] & r->regs[b]);  break;
      case 0x17 : r->regs[q] = op & 0x3f; break;
      case 0x18 : r->regs[q] = (r->regs[a] < r->regs[b])? r->regs[a] : r->regs[b]; break;
      case 0x19 : r->regs[q] = (r->regs[a] < r->regs[b])? r->regs[b] : r->regs[a]; break;
      case 0x1A : r->regs[q] = (r->regs[a] & r->regs[b]) == r->regs[b];         break;
      case 0x1B : r->regs[q] = (r->regs[a] & r->regs[b]) == r->regs[a];         break;
      case 0x1C : r->regs[q] = (r->regs[a] & r->regs[b]) == 0; break;
    }
  }
}




int main(){

}
