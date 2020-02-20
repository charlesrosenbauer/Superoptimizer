#include "core.h"
#include "interpreter.h"
#include "util.h"
#include "opcodes.h"









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
int superoptimize(PROGRAM* target, PROGRAM* candidates, int optct, TESTCASE* tests, int sizeLimit){



  return 0;
}
