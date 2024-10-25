#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

const int MAX_CHAR_SIZE = 63;
unsigned char preamble[] = {170, 170, 211, 145, 13};

int charToBin(unsigned char* dst, char c) {
  for (int i=0; i<8; i++) {
    dst[7-i] = ((c>>i)&1);
  }
  return 0;
}

int main(int argc, char* argv[]) {
  char data[] = "Hello, World!";
  unsigned char* binaryChar = malloc(8);
  unsigned char* encodedData = malloc(8*MAX_CHAR_SIZE);
  
  for (int i=0; i<strlen(data); i++) {
    charToBin(binaryChar, data[i]);
    strncpy(&encodedData[8*i], binaryChar, 8);
  }
  
  printf("%u\n", sizeof preamble);
  
  FILE *fptr;
  fptr = fopen("data.dat", "wb");
  fwrite(preamble, 5, 1, fptr);
  fwrite(data, 13, 1, fptr);
  fclose(fptr);
  return 0;
}
