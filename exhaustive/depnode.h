#ifndef __DEPNODE_HEADER_
#define __DEPNODE_HEADER_


#include "stdint.h"


typedef struct{
  uint8_t  a, b, c;
  uint8_t  size, inct, exct, opinit, readers;
  uint32_t exmask;
}DEPNODE;

typedef struct{
  DEPNODE  nodes[32];
  uint8_t  exs   [8];
  int      nodect, inct, exct;
}DEPLIST;




int      getMinSize  (int);
void     nextDeplist (DEPLIST*, uint64_t, int);
uint64_t getDepMap   (DEPLIST*);
void     printDeplist(DEPLIST*);


#endif
