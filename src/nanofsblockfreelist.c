#include "nanofsblockfreelist.h"

long getFirstFreeBlock(NanoFSBlockFreeList freeList);
void allocBlock(NanoFSBlockFreeList freeList, long offset);
void freeBlock(NanoFSBlockFreeList freeList, long offset);


long getFirstFreeBlock(NanoFSBlockFreeList freeList){
  for (long i = 0; i < BLOCK_FREE_LIST_DIMENSTION; i++){
    if (freeList[i] == ~0) continue;
    for (char j = 0; j < CHAR_BIT; j++){
      char a = (freeList[i] >> j);
      if (!a)
	return i*256+j;
    }
  }
  return -1;
}
void allocBlock(NanoFSBlockFreeList freeList, long offset) {
  long sectorsToScip = offset / 256;
  long blockRepresentingBit = offset % 8;
  freeList[sectorsToScip] |= (1 << blockRepresentingBit);
}
void freeBlock(NanoFSBlockFreeList freeList, long offset){
  long sectorsToScip = offset / 256;
  long blockRepresentingBit = offset % 8;
  freeList[sectorsToScip-1] &= ~(1 << blockRepresentingBit);
}

void printFreeBlockList(NanoFSBlockFreeList freeList){
  for (long i = 0; i < BLOCK_FREE_LIST_DIMENSTION; i++){
    printf("o:%ld %02x\n", i, (unsigned char)freeList[i]);
  }
}
