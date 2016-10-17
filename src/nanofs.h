#ifndef _NANOFS_H_
#define _NANOFS_H_
#include "globalsettings.h"
#include "disk.h"
#include "blockfreelist.h"
#include "inode.h"

typedef struct NanoFS {

  NanoFSBlockFreeList blockFreeList;
  inodeBitmap inodeBitmap;

} NanoFS;

#endif
