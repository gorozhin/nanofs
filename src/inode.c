#include "inode.h"
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

void printINode(inode in){
  printf("%s\n", in.fileName);
  printf("%ld\n", in.size);
  for (int i = 0; i < 64; i++) {
    if (!(unsigned long)in.offset[i]) break;
    printf("%ld\n", (unsigned long)in.offset[i]);
  }
  printf("\n");
}

void printINodeDefinedFile(NanoFSDisk disk, inode f){
  for (long i = 0; i < f.size; i++){
    long blocksToSkip = i / BLOCK_SIZE;
    long byteToPrint = i % BLOCK_SIZE;
    
    void* block = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(disk, f.offset[blocksToSkip], block);
    printf("%ld : %02x \n", i, ((unsigned char*)block)[byteToPrint]);
  }

}

//inode bitmap part
inodeBitmap readInodeBitmap(NanoFSDisk disk){
  inodeBitmap inbm = malloc(INODE_INDEX_MAP_BLOCKSIZE*BLOCK_SIZE);
  fseek(disk, (BLOCK_FREE_LIST_BLOCKSIZE+1)*BLOCK_SIZE, SEEK_SET);
  fread(inbm, sizeof(char)*BLOCK_SIZE*INODE_INDEX_MAP_BLOCKSIZE, 1, disk);
  rewind(disk);
  return inbm;
}

void writeInodeBitmap(NanoFSDisk disk, inodeBitmap bitmap){
  fseek(disk, (BLOCK_FREE_LIST_BLOCKSIZE+1)*BLOCK_SIZE, SEEK_SET);
  fwrite(bitmap, sizeof(char)*BLOCK_SIZE*INODE_INDEX_MAP_BLOCKSIZE, 1, disk);
  rewind(disk);
}

void allocInode(inodeBitmap bitmap, long offset) {
  long sectorsToScip = offset / 8;
  long blockRepresentingBit = (offset % 8);
  bitmap[sectorsToScip] |= (1 << blockRepresentingBit);
}

void freeInode(inodeBitmap bitmap, long offset){
  long sectorsToScip = offset / 8;
  long blockRepresentingBit = offset % 8;
  bitmap[sectorsToScip] &= ~(1 << blockRepresentingBit);
}

long getFirstFreeInode(inodeBitmap freeList){
  for (long i = 0; i < INODE_INDEX_MAP_DIMENSION; i++){
    if (freeList[i] == ~0) continue;
    for (char j = 0; j < CHAR_BIT; j++){
      char a = ((freeList[i] >> j) & 1);
      if (!a)
	return i*8+j;
    }
  }
  return -1;
}

long inodeOffsetToBlockOffset(long offset){
  return offset + INODE_INDEX_MAP_BLOCKSIZE + BLOCK_FREE_LIST_BLOCKSIZE +1 ;
}
