#ifndef _INODE_H_
#define _INODE_H_

#include "globalsettings.h"

typedef struct inode {
  //char fileName[MAX_FILENAME];
  long offset[1024];
}inode;

inode inodeFromBlock(void* block);

#endif
