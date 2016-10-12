#ifndef _NANOFS_BLOCK_FREE_LIST_H_
#define _NANOFS_BLOCK_FREE_LIST_H_
#include "nanofsglobalsettings.h"
#include <limits.h>
#include <stdio.h>

typedef char NanoFSBlockFreeList[BLOCK_FREE_LIST_DIMENSTION];

long getFirstFreeBlock(NanoFSBlockFreeList freeList);
void allocBlock(NanoFSBlockFreeList freeList, long offset);
void freeBlock(NanoFSBlockFreeList freeList, long offset);

#endif
