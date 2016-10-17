#include "nanofs.h"
#include <stdio.h>

int main() {
  NanoFSDisk d = openDisk("../tools/burn/file");
  inodeBitmap fbl = readInodeBitmap(d);
  NanoFSBlockFreeList fbll = readFreeBlockList(d);
  long i = 0, offset = 0;
  for (i = 0; i < INODE_INDEX_MAP_BLOCKSIZE*BLOCK_SIZE; i++) {
    printf("%x ", (unsigned char) *(fbl+i));
    offset++;
  }
  
  printf("\n\n\n");
  allocInode(fbl, 0);
  allocInode(fbl, 1);
  allocInode(fbl, 2);
  allocInode(fbl, 3);
  allocInode(fbl, 4);

  long o = getFirstFreeInode(fbl);
  long o1 = getFirstFreeBlock(fbll);

  writeInodeBitmap(d, fbl);
  i = 0, offset = 0;
  for (i = 0; i < INODE_INDEX_MAP_BLOCKSIZE*BLOCK_SIZE; i++) {
    printf("%x ", (unsigned char) *(fbl+i));
    offset++;
  }

  printf("\n%ld\n", offset);
  printf("\n%ld\n", o);
  printf("\n%ld\n", o1);
}
