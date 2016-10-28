#include "nanofs.h"
#include <stdio.h>
#include <string.h>  

int main() {
  NanoFSDisk d = openDisk("../tools/burn/file");
  NanoFS fs = mountNanoFS(d);

  printf("INODE_COUNT: %llu\n", INODE_COUNT);
  printf("INODE_COUNT_IDEAL: %llu\n", INODE_COUNT_IDEAL);

  for (int i = 0; i < INODE_COUNT; i++){
    char s[MAX_FILENAME];
    s[0] = 'a';
    s[1] = '0' + (i / 10000) % 10;
    s[2] = '0' + (i / 1000) % 10;
    s[3] = '0' + (i / 100) % 10;
    s[4] = '0' + (i / 10) % 10;
    s[5] = '0' + i % 10;
    s[6] = '\0';

    long ioffset = newFile(s, fs);
    if (ioffset < 0) 
      ioffset = findFile(s, fs);
    if (ioffset < 0) break;
    writeByteToFile(0x41, ioffset, fs);
    long offset = inodeOffsetToBlockOffset(ioffset);
    void* block = malloc(sizeof(char) * BLOCK_SIZE);
    readBlock(fs.disk, offset, block);
    inode* ind = (inode*)block;
    printf("%s : %ld %ld\n",ind->fileName, ioffset, offset);
    free(block);
  }
  printf("");
  long ioffset = 1;
  long i = 10000;
  do {
    char s[MAX_FILENAME];
    s[0] = 'a';
    s[1] = '0' + (i / 10000) % 10;
    s[2] = '0' + (i / 1000) % 10;
    s[3] = '0' + (i / 100) % 10;
    s[4] = '0' + (i / 10) % 10;
    s[5] = '0' + i % 10;
    s[6] = '\0';
    i++;
    ioffset = newFile(s, fs);
    printf("%ld\n", ioffset);
  }
  while(ioffset > 0);
  /* listFiles(fs); */
  unmountNanoFS(fs);
  fclose(d);
}

NanoFS mountNanoFS(NanoFSDisk d){
  NanoFS fs;

  fs.disk = d;
  fs.inodeBitmap = readInodeBitmap(d);
  fs.blockFreeList = readFreeBlockList(d);
  
  return fs;
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
	  free(block);
	  return (i*8+j);
	}
	free(block);
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
      if (firstFreeBlock < 0) {
	free(block);
	return;
      }
      allocBlock(fs.blockFreeList, firstFreeBlock);
      ind->offset[i] = firstFreeBlock;
      break;
    }
  }


  writeBlock(fs.disk, offset, block);
  syncNanoFS(fs);
  free(block);
}

void shrinkFile(long fileDescriptor, NanoFS fs){
  long ioffset = inodeOffsetToBlockOffset(fileDescriptor);
  void* block = malloc(sizeof(char) * BLOCK_SIZE);
  readBlock(fs.disk, ioffset, block);
  inode* ind = (inode*)block;
  for (int i = 63; i > 0; i--){
    if (ind->offset[i] != 0) {
      freeBlock(fs.blockFreeList, ind->offset[i]);
      ind->offset[i] = 0;
      break;
    }
  }
  free(block);
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
  free(block);
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
  
  if (blockToWrite > 63){
    free(block);
    return;
  }
  
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


void listFiles(NanoFS fs){
  for (uint64_t i = 0; i < INODE_COUNT; i++){
    for (uint64_t j = 0; j < 8; j++){
      if ((fs.inodeBitmap[i] >> j)  & 1) {
	void* block = malloc(sizeof(char) * BLOCK_SIZE);
	long offset = inodeOffsetToBlockOffset(i*8+j);
	readBlock(fs.disk, offset, block);
	inode* ind = (inode*)block;
	printf("%s\n", ind->fileName);

      }
    }
  }
}
