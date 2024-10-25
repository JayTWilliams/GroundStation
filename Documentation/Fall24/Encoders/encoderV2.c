#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

#include <time.h>

const int MAX_CHAR_SIZE = 63;
unsigned char preamble[] = {170, 170, 211, 145, 13};
//unsigned char preamble[] = {81,81,81,81,81};

int main(int argc, char* argv[]) {

	if (mkfifo("bitPipe", 0777) == -1) {
		//printf("%s\n", strerror(errno));
		if (errno != EEXIST) {
			printf("Could not create fifo file\n");
			return 1;
		}
	}
	
	printf("Opening...\n");
	int fd = open("bitPipe", O_WRONLY);
	if (fd == -1) {
		return 3;
	}
	printf("Opened\n");
	unsigned char x;

  unsigned char data[] = "Hello, World!";
  unsigned char* full = malloc(20);
  
  full = strcat(full, preamble);
  full = strcat(full, data);
  
  clock_t stop;
  char* t = "\0";
  
  while(1) {
    
    stop = clock() + 0.1*CLOCKS_PER_SEC;
    while (clock() < stop) {
      write(fd, t, 1);
    }
    
    for (int i=0; i<strlen(full); i++) {
      printf("%c", *(full+i));
      write(fd, full+i, 1);
    }
    printf("\n");
  }
  
  close(fd);
  return 0;
}
