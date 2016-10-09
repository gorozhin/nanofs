#ifndef _NNOFS_DISK_H_
#define _NNOFS_DISK_H_
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 32

typedef FILE* NanoFileSystemDisk;

/*
  Open a disk from a filename, return a NanoFileSystemDisk
*/
NanoFileSystemDisk openDisk(char* fileName);

/*
  Close NanoFileSystemDisk
*/
void closeDisk(NanoFileSystemDisk d);

/*
  Read a block (BLOCK_SIZE bytes) at a given offset from a disk,
  Return 0 if everything is Ok, or a non-zero error code
*/
char readBlock(NanoFileSystemDisk disk, long offset, void* block);

/*
  Write a block (BLOCK_SIZE bytes) starting at a given offset to a disk,
  Return 0 if everything is Ok, or a non-zero error code
*/
char writeBlock(NanoFileSystemDisk disk, long offset, void* block);


#endif //_NNOFS_DISK_H_
