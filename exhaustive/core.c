#include "core.h"
#include "interpreter.h"
#include "util.h"
#include "opcodes.h"










int superoptimize(PROGRAM* target, PROGRAM* candidates, int optct, TESTCASE* tests, int sizeLimit){

  int costLimit = 0;     // Get cost target for program.

  sizeLimit = (sizeLimit < 32)? sizeLimit : 32;

  int found = 0;

  { // Try 1 instructions

  }

  { // Try 2-4 instructions

  }

  for(int i = 5; i < sizeLimit; i++){

  }

  return 0;
}
