#ifndef _NNOFS_DISK_H_
#define _NNOFS_DISK_H_
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 32

typedef FILE* NanoFileSystemDisk;

NanoFileSystemDisk openDisk(char* fileName);
void closeDisk(NanoFileSystemDisk d);

char readBlock(NanoFileSystemDisk disk, long offset, void* block);
char writeBlock(NanoFileSystemDisk disk, long offset, void* block);


#endif //_NNOFS_DISK_H_
