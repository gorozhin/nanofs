#ifndef _NANOFS_BLOCK_FREE_LIST_H_
#define _NANOFS_BLOCK_FREE_LIST_H_
#include "globalsettings.h"
#include "disk.h"

#include <limits.h>
#include <stdio.h>

typedef char* NanoFSBlockFreeList;

long getFirstFreeBlock(NanoFSBlockFreeList freeList);
void allocBlock(NanoFSBlockFreeList freeList, long offset);
void freeBlock(NanoFSBlockFreeList freeList, long offset);
void* readFreeBlockList(NanoFSDisk disk);
void writeFreeBlockList(NanoFSBlockFreeList l, NanoFSDisk disk);

#endif
