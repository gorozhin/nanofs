#include <stdio.h>
#include <stdlib.h>
#include "../../src/disk.h"

// bytes to write, 1GB
#define N 1073741824

void burn(FILE* f){
  long offset = 0;
  for (long i = offset; i < BLOCK_SIZE; i++){
    fputc(0x0, f);offset++;
  }
  
  for (long i = offset; i < PROTECTED_BLOCKS / 8; i++){
    fputc(0xff, f);offset++;
  }
  fputc(PROTECTED_BLOCKS % 8, f);offset++;

  for (long i = offset; i < BLOCK_SIZE - (PROTECTED_BLOCKS / 8 + 1); i++){
    fputc(0x00, f);offset++;
  }
  
  for (long i = offset; i < N; i++){
    fputc(0x00, f);offset++;
  }				   
  
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
