#include "blockfreelist.h"

long getFirstFreeBlock(NanoFSBlockFreeList freeList);
void allocBlock(NanoFSBlockFreeList freeList, long offset);
void freeBlock(NanoFSBlockFreeList freeList, long offset);


long getFirstFreeBlock(NanoFSBlockFreeList freeList){
  for (long i = 0; i < BLOCK_FREE_LIST_DIMENSION; i++){
    if (freeList[i] == ~0) continue;
    for (char j = 0; j < CHAR_BIT; j++){
      char a = (freeList[i] >> j);
      if (!a)
	return i*8+j;
    }
  }
  return -1;
}
void allocBlock(NanoFSBlockFreeList freeList, long offset) {
  long sectorsToScip = offset / 8;
  long blockRepresentingBit = (offset % 8);
  freeList[sectorsToScip] |= (1 << blockRepresentingBit);
}
void freeBlock(NanoFSBlockFreeList freeList, long offset){
  long sectorsToScip = offset / 8;
  long blockRepresentingBit = offset % 8;
  freeList[sectorsToScip] &= ~(1 << blockRepresentingBit);
}

void printFreeBlockList(NanoFSBlockFreeList freeList){
  for (long i = 0; i < BLOCK_FREE_LIST_DIMENSION; i++){
    printf("o:%ld %02x\n", i, (unsigned char)freeList[i]);
  }
}

int main(){
  NanoFSBlockFreeList a;
  for (long i = 0; i < BLOCK_FREE_LIST_DIMENSION; i++) a[i] = 0;
  for (long i = 0; i < BLOCKS_COUNT - 10; i++) 
    allocBlock(a,i);
  printf("%ld\n", getFirstFreeBlock(a));
  /* allocBlock(a,(MAX_DISK_SIZE / BLOCK_SIZE) - 1); */
  printf("%d : %x \n",(BLOCK_FREE_LIST_DIMENSION - 1), (unsigned char)a[BLOCK_FREE_LIST_DIMENSION - 1]);
  
  /* printf("%d\n", a[0] & 1); */
  /* allocBlock(a,0); */
  /* printf("%d\n", a[0] & 1); */

  /* printf("%d\n", (a[0]>>1) & 0b1); */
  /* allocBlock(a,1); */
  /* printf("%d\n", (a[0]>>1) & 0b1); */

  /* printf("%d\n", (a[127]>>7) & 0b1); */
  /* allocBlock(a,262144); */
  /* printf("%d\n", (a[127]>>7) & 0b1); */
}
