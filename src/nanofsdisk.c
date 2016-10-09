#include "nanofsdisk.h"

NanoFileSystemDisk openDisk(char* name) {
  return fopen(name,"r+b");
}

void closeDisk(NanoFileSystemDisk d){
  fclose((FILE*)d);
}

char readBlock(NanoFileSystemDisk disk, long offset, void* block){
  fseek(disk, offset*BLOCK_SIZE, SEEK_SET);
  fread(block, sizeof(char)*BLOCK_SIZE, 1, disk);
  rewind(disk);
  return 0;
}

char writeBlock(NanoFileSystemDisk disk, long offset, void* block){
  fseek(disk, offset*BLOCK_SIZE, SEEK_SET);
  fwrite(block, sizeof(char)*BLOCK_SIZE, 1, disk);
  rewind(disk);
  return 0;
}


// NOT for implementation, delete on 
void printBlock(void* block){
  for (int i = 0; i < BLOCK_SIZE; i++)
    printf("%02x ", *((unsigned char*)(block+i)));
}

// this is a header file, so main should be deleted from final version,
// right now this is a testing polygone
int main(){
  NanoFileSystemDisk d = openDisk("../tools/burn/file");
  if (d == NULL){
    printf("error opening file, please check \"../tools/burn/file\"\n");
    return 1;
  }
  void* block = malloc(BLOCK_SIZE * sizeof(char));
  void* block1 = malloc(BLOCK_SIZE * sizeof(char));
  for (char i = 0; i < BLOCK_SIZE; i++)
    *((unsigned char*)(block1+i)) = i;

  readBlock(d, 0, block);
  writeBlock(d, 0, block1);
  readBlock(d, 0, block1);
  writeBlock(d, 0, block);

  printBlock(block);
  printf("\n");
  printBlock(block1);
  printf("\n");
  
  free(block);
  free(block1);
  closeDisk(d);
  return 0;
}
