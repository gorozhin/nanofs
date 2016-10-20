#include <stdio.h>
#include <stdlib.h>
#include "../../src/disk.h"
#include "../../src/nanofs.h"
#include "../../src/blockfreelist.h"
#include "../../src/inode.h"

// bytes to write, 1GB
#define N 1073741824

void burn(FILE* f){
  for (long i = 0; i < N; i++){
    fputc(0x00, f);
  }
  NanoFSBlockFreeList fbl = readFreeBlockList(f);
  for (long i = 0; i < PROTECTED_BLOCKS; i++){
    allocBlock(fbl, i);
  }
  writeFreeBlockList(fbl, f);
}

void read(FILE* f){
  unsigned char toPrint;
  printf("\t\t ");
  for(int i = 0;i < 10; i++){
    printf(" %d ", i);
  }
  for (long int i = 0; i < N; i++){
    toPrint = fgetc(f);
    if (!(i % 10)) printf("\n%ld\t\t ", i / 10);
    printf("%02x ", toPrint);
  }
  printf("\n");
}

int main(){
  int rw = 1;
  FILE* f = fopen("file", "r+b");
  if (f == NULL) {
    f = fopen("file", "wb");
    if (f == NULL) {
      printf("error acessing file\n");
      return 1;
    }
    f = freopen("file", "r+b", f);
  }
  char c;
  for(;;){
    printf("c: ");
    c = fgetc(stdin);
    fseek(stdin, 0, SEEK_END);
    if (c == EOF || c == 'q' || c == 'Q'){
      break;
    } else if (c == 'b' || c == 'B'){
      burn(f);
      f = freopen("file", "r+b", f);
    } else if (c == 'r' || c == 'R'){
      read(f);
      f = freopen("file", "r+b", f);
    }
  }

  fclose(f);
  return 0;
}
