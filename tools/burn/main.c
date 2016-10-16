#include <stdio.h>
#include <stdlib.h>

// bytes to write, 1GB
#define N 1073741824

void burn(FILE* f){
  long offset = 0;
  
  for(long int i = offset; i < 4096; i++){
    fputc(0, f);offset++;
  }
  
  for(long int i = 0; i < 16; i++){
    fputc(0xff, f);offset++;
  }

  for(long int i = 0; i < 16; i++){
    fputc(0xff, f);offset++;
  }
  
  fputc(0x9, f); offset++;
  
  for(long int i = 0; i < 8*4096-17; i++){
    fputc(0x0, f);offset++;
  }
  
  for(long int i = offset; i < N; i++){
    fputc(0, f);offset++;
  }
  /* long offset = 0; */
  /* unsigned char toWrite = 1; */
  
  /* fputc(0x42, f);offset++; */
  /* fputc(0x43, f);offset++; */
  /* fputc(0x44, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */

  /* fputc(0x1, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */

  /* fputc(0x2, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */
  /* fputc(0x0, f);offset++; */


  /* for(long int i = 0; i < 62; i++){ */
  /*     fputc(0x0, f);offset++; */
  /*     fputc(0x0, f);offset++; */
  /*     fputc(0x0, f);offset++; */
  /*     fputc(0x0, f);offset++; */
  /*     fputc(0x0, f);offset++; */
  /*     fputc(0x0, f);offset++; */
  /*     fputc(0x0, f);offset++; */
  /* } */
  /* for (long int i = offset; i < 4096; i++){ */
  /*   fputc(0x0, f);offset++; */
  /* } */

  /* toWrite = 0x41; */
  /* for (long int i = offset, j = offset+4096; i < j; i++){ */
  /*   fputc(toWrite, f); */
  /*   toWrite++; */
  /*   offset++; */
  /*   (toWrite > 0x5a) ? toWrite = 0x41: 1; */
  /* } */

  /* for (long int i = offset, j = offset+1096; i < j; i++){ */
  /*   fputc(0x41, f); */
  /*   offset++; */
  /* } */

  /* for (long int i = offset, j = offset+3000; i < j; i++){ */
  /*   fputc(0x0, f); */
  /*   offset++; */
  /* } */
  
  /* for (long int i = offset; i < N; i++){ */
  /*   fputc(0, f); */
  /*   /\* fputc(toWrite, f); *\/ */
  /*   /\* toWrite++; *\/ */
  /*   /\* if (!toWrite) toWrite = 1; *\/ */
  /* } */
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
