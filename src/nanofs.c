#include "nanofs.h"
#include <stdio.h>
#include <string.h>  

int main() {
  NanoFSDisk d = openDisk("../tools/burn/file");
  NanoFS fs = mountNanoFS(d);
  
  long ioffset = newFile("abcdef", fs);
  if (ioffset < 0) return 1;
  printf("IOFFSET:%ld\n", ioffset);
  
  long offset = inodeOffsetToBlockOffset(ioffset);
  printf("OFFSET:%ld\n", offset);

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
    
  printf("\n%d\n",fileExists("asdf", fs));
  printf("\n%d\n",fileExists("abcdef", fs));
  
  syncNanoFS(fs);
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
  inodeBitmap inbm = fs.inodeBitmap;
  for (long i = 0; i < INODE_INDEX_MAP_DIMENSION; i++){
    for (char j = 0; j < CHAR_BIT; j++){
      if((inbm[i] >> j) & 1) {
	void* block = malloc(sizeof(char) * BLOCK_SIZE);
	readBlock(fs.disk, inodeOffsetToBlockOffset(i*8+j), block);
	inode ind = inodeFromBlock(block);
	if (strcmp(ind.fileName, name) == 0){
	  return 1;
	}
      }
    }
  }
  return 0;
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
  return placeToWrtie;
}
