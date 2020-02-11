#include "stdint.h"
#include "stdio.h"
#include "opcodes.h"
#include "util.h"







int main(){

  PROGRAM p;
  OPCODETABLE tab = makeOpcodeTable();

  p.ops[0] = newUnop (PCT , 4);
  p.ops[1] = newBinop(ADDI, 4, 5);
  p.ops[2] = newBinop(ADDI, 1, 4);
  p.ops[3] = newBinop(MULI, 4, 6);
  p.inct   = 4;
  p.exct   = 4;
  p.exs[0] = 0;
  p.exs[1] = 1;
  p.exs[2] = 2;
  p.exs[3] = 3;
  p.size   = 4;

  uint64_t depmap = getDependencyMap(&tab, &p);
  printU64(depmap);

}
