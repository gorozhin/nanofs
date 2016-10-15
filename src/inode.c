#include "inode.h"
#include "globalsettings.h"
#include "disk.h"
#include <stdio.h>
#include <stdlib.h>

inode inodeFromBlock(void* block);

int main() {
  //  NanoFSDisk disk = openDisk("../tools/burn/file");
  //void* block = malloc(BLOCK_SIZE);
  //readBlock(disk, 0, block);
  //for (long i = 0; i < BLOCK_SIZE; i++){
  //  printf("%x ", *(unsigned char*)(block+i));
  //}
  //printf("\n");
  printf("%d\n", sizeof(inode));
  //closeDisk(disk);
  return 0;
}
