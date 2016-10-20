#ifndef _INODE_H_
#define _INODE_H_
#include "globalsettings.h"
#include "disk.h"

typedef char* inodeBitmap;

typedef struct inode {
  char fileName[MAX_FILENAME];
  unsigned long size;
  unsigned long offset[64];
} inode;


inodeBitmap readInodeBitmap(NanoFSDisk disk);
void allocInode(inodeBitmap bitmap, long offset);
void freeInode(inodeBitmap bitmap, long offset);

void writeInodeBitmap(NanoFSDisk disk, inodeBitmap bitmap);
inode inodeFromBlock(void* block);
void printINode(inode in);
void printINodeDefinedFile(NanoFSDisk disk, inode f);
long getFirstFreeInode(inodeBitmap freeList);
long inodeOffsetToBlockOffset(long offset);


#endif
