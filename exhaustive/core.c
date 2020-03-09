#include "core.h"
#include "interpreter.h"
#include "util.h"
#include "opcodes.h"







int selectOpType(OPCODETABLE* tab, PROGRAM* p, PROGRAMDATA* pd, int opix){
  switch(pd->optyps[opix]){
    //OP_1_1, OP_2_1, OP_2_2, OP_3_1, OP_INPUT
    case OP_1_1 : {
      pd->optyps[opix] = OP_2_1;
    }break;

    case OP_2_1 : {
      pd->optyps[opix] = OP_2_2;
    }break;

    case OP_2_2 : {
      pd->optyps[opix] = OP_3_1;
    }break;

    case OP_3_1 : {
      pd->optyps[opix] = OP_1_1;
      return 1;
    }break;

    default: return 1;
  }

  return 0;
}



int selectDeps(OPCODETABLE* tab, PROGRAM* p, PROGRAMDATA* pd, int opix){
  switch(pd->optyps[opix]){
    case OP_1_1 : {
      // Either single-use or multi-use
    }break;

    case OP_2_1 : {
      // Either single-use or multi-use
    }break;

    case OP_2_2 : {
      // A: Either single-use or multi-use
      // B: Either single-use, multi-use, or unused. Unused constrains the
      //      opcodes that can be used. Mostly only divmod and multiply can be
      //      used in that case.
    }break;

    case OP_3_1 : {
      // Either single-use or multi-use
      return 1;
    }break;

    case OP_INPUT : {
      // Either single-use or multi-use
    }break;
  }

  return 0;
}



int selectOpcode(OPCODETABLE* tab, PROGRAM* p, PROGRAMDATA* pd, int opix){


  return 0;
}





/*
  PLAN:

  1. This will be a depth-first search through the space of possible programs.
    It will start at the minimum determined size for the program, and begin to
    grow from there.

  2. Dependencies between opcodes will be mapped, and the DAG will be required
    to remain canonical. Outputs of opcodes will be labeled as UNUSED,
    SINGLE_USE, or MULTI_USE. This places constraints on future dependencies,
    which will cause backtracking. Additional dependency tracking can be done
    to constrain this even further, such as checking that the dependency map of
    each instruction is a subset of the dependency map of a final output.

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
    int newOp = 0;// nextOpcode(tab, &opt, stackhead);
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
