#include "nanofs.h"

int main(){
  NanoFSBlockFreeList a;
  for (long i = 0; i < BLOCK_FREE_LIST_DIMENSTION; i++) a[i] = 0;
  for (int i = 0; i < 256; i++)
    allocBlock(a,i);
  
  printf("%ld\n", getFirstFreeBlock(a));

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
