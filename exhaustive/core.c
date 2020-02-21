#include "core.h"
#include "interpreter.h"
#include "util.h"
#include "opcodes.h"







int nextOpcode(OPCODETABLE* tab, PROGRAM* p, int opix){
  OPCODE op = p->ops[opix];
  if(op.op > tab->maxOpIx){
    op.op = tab->minOpIx;
    int ok = 0;
    if      (isUnop  (tab, op.op)){
      // pick new a
    }else if(isBinop (tab, op.op)){
      // pick new a & b
    }else if(isTrinop(tab, op.op)){
      // pick new a & b & c
    }
    if(ok){
      p->ops[opix] = op;
      return 1;
    }
  }else{
    // Change this to take into account whether or not the opcode is defined.
    op.op++;
  }

  return 0;
}





/*
  PLAN:

  1. This will be a depth-first search through the space of possible programs.
    It will start at the minimum determined size for the program, and begin to
    grow from there.

  2. The program will be split into several subprograms with a variety of rules
    for mapping dependencies between them. If the dependency map does not match
    the original program well, it will be rejected on the grounds that dependency
    analysis has shown it to be incapable of generating a correct program. This
    will eliminate large classes of programs.

  3. Short subsequences of instructions can be analyzed for efficiency. If there
    is a sequence of 2 or 3 instructions that is known to be able to be equivalent
    to a more efficient implementation, or that includes non-canonical opcodes
    (e.g, a + b is allowed, but not b + a), that path can be pruned.

  4. The order in which subtrees are visited can be changed by a learned reordering
    function. This will allow for programs that are more likely to be equivalent
    to the original program to be tested sooner than those that are not.

  5. Test cases can be generated at each level of the tree. This reduces the cost
    of generating tests for individual optimization candidates. It also allows for
    monte carlo approximations of many hard correctness tests to be applied.

  6. By utilizing monte carlo test cases, equivalence partitions can be detected
    in subtrees. Because g(x) == g(y) => f(g(x)) == f(g(y)), we can assert that
    a subtree that places two results into the same partition when they are not
    in the same partition in the final result must be incorrect. This is potentially
    a very powerful optimization, as it not only filters out inefficient
    optimization candidates, but also filters out optimization candidates that
    simply cannot implement the specific optimization target.


*/
int superoptimize(OPCODETABLE* tab, PROGRAM* target, PROGRAM* candidates, int optct, TESTCASE* tests, int sizeLimit){

  /*
    sigpars    = getSigPars(target);
    stacklimit = sigpars / 2;
    if(stacklimit > sizeLimit) return 0;
  */

  PROGRAM opt;

  int cont = 1;
  int stackhead   = 0;
  int stacklimit  = 1;   // Eventually start this at (sigpars / 2)
  int candidatect = 0;
  while(cont){
    int newOp = nextOpcode(tab, &opt, stackhead);
    if(newOp){
      stackhead--;
    }else{
      int pass = 0;
      // pick opcode
      // pass = run pretest opcode filters
      if(pass){
        // run tests
        // pass = run posttest opcode filters
        if(pass){
          stackhead++;
        }
      }

      if(stackhead == sizeLimit){
        int pass = 0;
        // pass = test full optimization
        if(pass){
          int valid = 1;
          // valid = verify optimization
          if(valid){
            candidates[candidatect] = opt;
            candidatect++;
            if(candidatect == optct){
              return candidatect;
            }
          }
        }
      }
    }
  }

  return candidatect;
}
