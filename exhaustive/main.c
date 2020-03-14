#include "stdint.h"
#include "stdio.h"
#include "opcodes.h"
#include "util.h"
#include "core.h"







int main(){

  PROGRAM p;
  OPCODETABLE tab = makeOpcodeTable();

  p.ops[0] = newUnop (PCT ,  0);
  p.ops[1] = newBinop(ADDI,  2, 0);
  p.ops[2] = newBinop(ADDI, 16, 4);
  p.ops[3] = newBinop(MULI,  6, 0);
  p.inct   =  4;
  p.exct   =  4;
  p.exs[0] = 16;
  p.exs[1] = 18;
  p.exs[2] = 20;
  p.exs[3] = 22;
  p.size   =  4;

  uint64_t depmap = getDependencyMap(&tab, &p);
  printU64(depmap);

  PROGRAM opts[16];
  TESTCASE tests = makeTests(&tab, &p, 256);

  for(int i = 0; i < 48; i++){
    printf("%i : %lu\n", i, tests.tests[i].aRets[0].U64);
  }

  //superoptimize(&tab, &p, opts, 16, &tests, 256);

}
