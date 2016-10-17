#include "nanofs.h"
#include <stdio.h>
//~/Developer/c/dvfu/nanofs/src/
int main() {
  NanoFSDisk d = openDisk("../tools/burn/file");
  /* void* block = malloc(sizeof(char)*BLOCK_SIZE); */
  /* for(long i = 0; i < BLOCK_SIZE; i++){ */
  /*   *(char*)(block+i) = 0x41; */
  /* } */

  
  /* writeBlock(d, 101, block); */

  NanoFS fs = mountNanoFS(d);
  long ioffset = newFile("abcdef", fs);
  long offset = inodeOffsetToBlockOffset(ioffset);
  void* block = malloc(sizeof(char)*BLOCK_SIZE);
  readBlock(fs.disk, offset, block);
  inode ind = inodeFromBlock(block);

  printf("INODE:\n");
  printINode(ind);
  printf("\n\nINODE DEFINED FILE:\n");
  printINodeDefinedFile(d, ind);

  void* block1 = malloc(sizeof(char)*BLOCK_SIZE);
  readBlock(d, ind.offset[0], block1);
  printBlock(block1);

  free(block);
  free(block1);
  unmountNanoFS(fs);
  fclose(d);
}

NanoFS mountNanoFS(NanoFSDisk d){
  NanoFS r;

  r.disk = d;
  r.inodeBitmap = readInodeBitmap(d);
  r.blockFreeList = readFreeBlockList(d);
  
  return r;
}

void unmountNanoFS(NanoFS fs){
  free(fs.inodeBitmap);
  free(fs.blockFreeList);
}

void syncNanoFS(NanoFS fs){
  writeInodeBitmap(fs.disk, fs.inodeBitmap);
  writeFreeBlockList(fs.blockFreeList, fs.disk);
}

long newFile(char name[MAX_FILENAME], NanoFS fs){
  void* block = malloc(sizeof(char)*BLOCK_SIZE);
  for(long i = 0; i < BLOCK_SIZE; i++){
    *(char*)(block+i) = 0;
  }
  
  inode* newInode = (inode*)block;

  for(char i = 0; i < 8; i++)
    newInode->fileName[i] = name[i];
  
  newInode->size = 0;
  for (int i = 0; i < 64; i++)
    newInode->offset[i] = 0;
  
  long inodeIndex = getFirstFreeInode(fs.inodeBitmap);
  allocInode(fs.inodeBitmap, inodeIndex);


  long firstBlockIndex = getFirstFreeBlock(fs.blockFreeList);
  allocBlock(fs.blockFreeList, firstBlockIndex);
  
  newInode->offset[0] = firstBlockIndex;

  
  long placeToWrtie = inodeOffsetToBlockOffset(inodeIndex);
  writeBlock(fs.disk,placeToWrtie, block);
 
  syncNanoFS(fs);
  free(block);
  return 0;
}
