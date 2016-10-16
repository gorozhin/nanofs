#include "nanofs.h"
#include <stdio.h>

int main() {
  NanoFSDisk d = openDisk("../tools/burn/file");
  NanoFSBlockFreeList fbl = readFreeBlockList(d);

  /* void* block = malloc(BLOCK_SIZE); */
  /* readBlock(d, 0, block); */
  
  /* for (long i = 0; i < BLOCK_SIZE; i++) */
  /*   printf("%x ", *(unsigned char*)(block+i)); */

  for (long i = 0; i < BLOCK_FREE_LIST_BLOCKSIZE*BLOCK_SIZE; i++)
    printf("%x ", (unsigned char)*(fbl+i));

  printf("\nHello from nanofs!\n");
}
