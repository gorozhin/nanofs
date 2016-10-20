#include "nanofs.h"
#include <stdio.h>
#include <string.h>  

int main() {
  NanoFSDisk d = openDisk("../tools/burn/file");
  NanoFS fs = mountNanoFS(d);
  
  /* long ioffset = newFile("abcde", fs); */
  /* if (ioffset < 0) ioffset = findFile("abcde", fs); */

  /* for (long i = 0; i < 4096*64; i++) */
  /*   writeByteToFile(0x41+(i % (0x5a-0x41)), ioffset, fs); */

  /* long offset = inodeOffsetToBlockOffset(ioffset); */
  /* void* block = malloc(sizeof(char) * BLOCK_SIZE); */
  /* readBlock(fs.disk, offset, block); */
  /* inode* ind = (inode*)block; */

  /* printINode(*ind); */
  /* printINodeDefinedFile(fs.disk, *ind); */
  
  long ioffset1 = newFile("a1", fs);
  long ioffset2= newFile("a2", fs);

  writeByteToFile(0x41, ioffset1, fs);
  writeByteToFile(0x42, ioffset2, fs);

  long offset = inodeOffsetToBlockOffset(ioffset1);
  void* block = malloc(sizeof(char) * BLOCK_SIZE);
  readBlock(fs.disk, offset, block);
  inode* ind = (inode*)block;

  printf("a1:\n");
  printINodeDefinedFile(fs.disk, *ind);
  
  long offset1 = inodeOffsetToBlockOffset(ioffset2);
  void* block1 = malloc(sizeof(char) * BLOCK_SIZE);
  readBlock(fs.disk, offset1, block1);
  inode* ind1 = (inode*)block1;

  printf("a2:\n");
  printINodeDefinedFile(fs.disk, *ind1);
  
  for (long i = 0; i < 4096; i++)
    writeByteToFile(0x41+(i % (0x5a-0x41)), ioffset1, fs);

  readBlock(fs.disk, offset, block);
  ind = (inode*)block;
  printf("a1:\n");
  printINodeDefinedFile(fs.disk, *ind);
  printf("a2:\n"); 
  printINodeDefinedFile(fs.disk, *ind1);


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

char fileExists(char name[MAX_FILENAME], NanoFS fs){
  return (findFile(name, fs) != -1);
}

long findFile(char name[MAX_FILENAME], NanoFS fs){
  inodeBitmap inbm = fs.inodeBitmap;
  for (long i = 0; i < INODE_INDEX_MAP_DIMENSION; i++){
    for (char j = 0; j < CHAR_BIT; j++){
      if((inbm[i] >> j) & 1) {
	void* block = malloc(sizeof(char) * BLOCK_SIZE);
	readBlock(fs.disk, inodeOffsetToBlockOffset(i*8+j), block);
	inode* ind = (inode*)block;
	if (strcmp(ind->fileName, name) == 0){
	  return (i*8+j);
	}
      }
    }
  }
  return -1;
}

void growFile(long fileDescriptor, NanoFS fs){
  long offset = inodeOffsetToBlockOffset(fileDescriptor);
  void* block = malloc(sizeof(char) * BLOCK_SIZE);
  readBlock(fs.disk, offset, block);
  inode* ind = (inode*)block;

  for (int i = 0; i < 64; i++){
    if(ind->offset[i] == 0){
      long firstFreeBlock = getFirstFreeBlock(fs.blockFreeList);
      printf("firstFreeBlock: %ld\n", firstFreeBlock);
      if (firstFreeBlock < 0) return;
      allocBlock(fs.blockFreeList, firstFreeBlock);
      ind->offset[i] = firstFreeBlock;
      break;
    }
  }

  writeBlock(fs.disk, offset, block);
  syncNanoFS(fs);
}

void shrinkFile(long fileDescriptor, NanoFS fs){
  long ioffset = inodeOffsetToBlockOffset(fileDescriptor);
  void* block = malloc(sizeof(char) * BLOCK_SIZE);
  readBlock(fs.disk, ioffset, block);
  inode* ind = (inode*)block;
  for (int i = 63; i > 0; i--){
    if(ind->offset[i] != 0){
      freeBlock(fs.blockFreeList, ind->offset[i]);
      ind->offset[i] = 0;
      break;
    }
  }
  writeBlock(fs.disk, ioffset, block);
  syncNanoFS(fs);
}

void deleteFile(char name[MAX_FILENAME], NanoFS fs){
  long ioffset = findFile(name, fs);
  void* block = malloc(sizeof(char) * BLOCK_SIZE);
  long offset = inodeOffsetToBlockOffset(ioffset);
  readBlock(fs.disk, offset, block);
  inode* ind = (inode*)block;
  for (int i = 0; i < 64; i++){
    if(ind->offset[i] == 0) break;
    freeBlock(fs.blockFreeList, ind->offset[i]);
    ind->offset[i] = 0;
  }
  writeBlock(fs.disk, offset, block);
  freeInode(fs.inodeBitmap, ioffset);
  syncNanoFS(fs);
}

long newFile(char name[MAX_FILENAME], NanoFS fs){
  if (fileExists(name, fs)) return -1;
  
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
  writeBlock(fs.disk, placeToWrtie, block);
  
  syncNanoFS(fs);
  free(block);
  return inodeIndex;
}

void writeByteToFile(char byte, long fileDescriptor, NanoFS fs) {
  void* block = malloc(sizeof(char) * BLOCK_SIZE);
  long offset = inodeOffsetToBlockOffset(fileDescriptor);
  readBlock(fs.disk, offset, block);
  inode* ind = (inode*)block;
  
  long blockToWrite = (ind->size / BLOCK_SIZE);
  long byteToWrite = (ind->size % BLOCK_SIZE);
  
  if (blockToWrite > 63)
    return;

  if (!ind->offset[blockToWrite]){
    growFile(fileDescriptor, fs);
    readBlock(fs.disk, offset, block);
    ind = (inode*)block;
  }
  ind->size = ind->size + 1;

  writeBlock(fs.disk, offset, block);

  void* block1 = malloc(sizeof(char) * BLOCK_SIZE);
  readBlock(fs.disk, ind->offset[blockToWrite], block1);
  *(char*)(block1+byteToWrite) = byte;
  writeBlock(fs.disk, ind->offset[blockToWrite], block1);
  
  free(block);
  free(block1);
}
