#include "disk.h"

NanoFSDisk openDisk(char* name) {
  return fopen(name,"r+b");
}

void closeDisk(NanoFSDisk d){
  fclose((FILE*)d);
}

char readBlock(NanoFSDisk disk, long offset, void* block){
  fseek(disk, offset*BLOCK_SIZE, SEEK_SET);
  fread(block, sizeof(char)*BLOCK_SIZE, 1, disk);
  rewind(disk);
  return 0;
}

char writeBlock(NanoFSDisk disk, long offset, void* block){
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
