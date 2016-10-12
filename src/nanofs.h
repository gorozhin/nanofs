#ifndef _NANOFS_H_
#define _NANOFS_H_
#include "nanofsglobalsettings.h"
#include "nanofsdisk.h"
#include "nanofsblockfreelist.h"

typedef struct NanoFSINode{
  char name[MAX_FILENAME];
  
} NanoFSINode;

typedef struct NanoFSHeader{
  NanoFSBlockFreeList blockFreeList;
  // TODO: add more meta to the definition
} NanoFSHeader;


#endif
