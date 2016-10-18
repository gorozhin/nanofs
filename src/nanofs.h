#ifndef _NANOFS_H_
#define _NANOFS_H_
#include "globalsettings.h"
#include "disk.h"
#include "blockfreelist.h"
#include "inode.h"

typedef struct NanoFS {
  
  NanoFSBlockFreeList blockFreeList;
  inodeBitmap inodeBitmap;
  NanoFSDisk disk;
  
} NanoFS;


NanoFS mountNanoFS(NanoFSDisk d);
void unmountNanoFS(NanoFS fs);
void syncNanoFS(NanoFS fs);
long newFile(char name[MAX_FILENAME], NanoFS fs);
char fileExists(char name[MAX_FILENAME], NanoFS fs);


#endif
