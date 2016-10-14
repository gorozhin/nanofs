#ifndef _NANOFS_H_
#define _NANOFS_H_
#include "globalsettings.h"
#include "disk.h"
#include "blockfreelist.h"

typedef struct NanoFSINode{
  char name[MAX_FILENAME];
  
} NanoFSINode;

typedef struct NanoFSHeader{
  NanoFSBlockFreeList blockFreeList;
  // TODO: add more meta to the definition
} NanoFSHeader;


#endif
