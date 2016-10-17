#include "blockfreelist.h"

long getFirstFreeBlock(NanoFSBlockFreeList freeList);
void allocBlock(NanoFSBlockFreeList freeList, long offset);
void freeBlock(NanoFSBlockFreeList freeList, long offset);

long getFirstFreeBlock(NanoFSBlockFreeList freeList){
  for (long i = 0; i < BLOCK_FREE_LIST_DIMENSION; i++){
    if (freeList[i] == ~0) continue;
    for (char j = 0; j < CHAR_BIT; j++){
      char a = ((freeList[i] >> j) &1);
      if (!a)
	return i*8+j;
    }
  }
  return -1;
}

void allocBlock(NanoFSBlockFreeList freeList, long offset) {
  long sectorsToScip = offset / 8;
  long blockRepresentingBit = (offset % 8);
  freeList[sectorsToScip] |= (1 << blockRepresentingBit);
}

void freeBlock(NanoFSBlockFreeList freeList, long offset){
  long sectorsToScip = offset / 8;
  long blockRepresentingBit = offset % 8;
  freeList[sectorsToScip] &= ~(1 << blockRepresentingBit);
}

void printFreeBlockList(NanoFSBlockFreeList freeList){
  for (long i = 0; i < BLOCK_FREE_LIST_DIMENSION; i++){
    printf("o:%ld %02x\n", i, (unsigned char)freeList[i]);
  }
}

NanoFSBlockFreeList readFreeBlockList(NanoFSDisk disk){
  fseek(disk, 1*BLOCK_SIZE, SEEK_SET);
  void* block = malloc(BLOCK_FREE_LIST_BLOCKSIZE*BLOCK_SIZE);
  fread(block, sizeof(char)*BLOCK_FREE_LIST_BLOCKSIZE*BLOCK_SIZE, 1, disk);
  rewind(disk);
  return block;
}

void writeFreeBlockList(NanoFSBlockFreeList l, NanoFSDisk disk){
  fseek(disk, 1*BLOCK_SIZE, SEEK_SET);
  fwrite(l, sizeof(char)*BLOCK_FREE_LIST_BLOCKSIZE*BLOCK_SIZE, 1, disk);
  rewind(disk);
}
