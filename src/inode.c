#include "inode.h"
#include <stdio.h>
#include <stdlib.h>

void printINode(inode in){
  printf("%s\n", in.fileName);
  for (int i = 0; i < 64; i++) {
    printf("%lx\n", (unsigned long)in.offset[i]);
  }
  printf("\n");

}

inode inodeFromBlock(void* block){
  return *(inode*)block;
}

void printINodeDefinedFile(NanoFSDisk disk, inode f){
  for (int i = 0; i < 64; i++){
    long offset = f.offset[i];
    if (offset <= 0){
      break;
    }

    void* block = malloc(BLOCK_SIZE);
    readBlock(disk, offset, block);

    for (int j = 0; j < 4096; j++)
      printf("%c", *(char*)(block+j));
  }
}
