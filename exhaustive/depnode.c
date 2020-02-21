#include "depnode.h"
#include "stdio.h"
#include "util.h"




inline int getMinSize(int relevantParct){
  return relevantParct / 2;
}

/*
  RULES:

  * 1-3 inputs per node

  * first operation must use all inputs

  * each additional operation must depend on the previous opcode

  * final result must be an output

  * if only 1 input, it must come from an internal result or a parameter

  * all outputs of all nodes must be used

*/
void nextDepList(DEPLIST* deplist, uint64_t depmap, int opct){

  for(int i = 0; i < deplist->nodect; i++){
    
  }
}


uint64_t getDepMap(DEPLIST* deplist){
  uint64_t ret = 0;
  uint64_t map[40];
  for(int i = 0; i < 8; i++) map[i] = 1l << i;
  for(int i = 0; i < deplist->nodect; i++){
    DEPNODE  n = deplist->nodes[i];
    uint64_t a = 0;
    uint64_t b = 0;
    uint64_t c = 0;
    switch(n.inct){
      case 1:{
        a = map[n.a];
      }break;
      case 2:{
        a = map[n.a];
        b = map[n.b];
      }break;
      case 3:{
        a = map[n.a];
        b = map[n.b];
        c = map[n.c];
      }break;
    }
    map[i+8] = (a | b | c);
  }

  for(int i = 0; i < deplist->exct; i++){
    ret |= (map[deplist->exs[i]+8] << (8 * i));
  }

  return ret;
}


void printDeplist(DEPLIST* deplist){
  for(int i = 0; i < deplist->nodect; i++){
    DEPNODE n = deplist->nodes[i];
    printf("%i %i : %i %i %i => ", n.inct, n.exct, n.a, n.b, n.c);
    printU32(n.exmask);
  }
}
