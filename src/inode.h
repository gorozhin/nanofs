#ifndef _INODE_H_
#define _INODE_H_

#include "globalsettings.h"

typedef struct inode {
  char fileName[MAX_FILENAME];
  //unsigned long sizeOfFile; 
  unsigned long offset[64];
} inode;

inode inodeFromBlock(void* block);
void printINode(inode in);

#endif
