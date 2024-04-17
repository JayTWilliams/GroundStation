#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
	if (mkfifo("bitPipe", 0777) == -1) {
		//printf("%s\n", strerror(errno));
		if (errno != EEXIST) {
			printf("Could not create fifo file\n");
			return 1;
		}
	}
	
	printf("Opening...\n");
	int fd = open("bitPipe", O_RDWR);
	if (fd == -1) {
		return 3;
	}
	printf("Opened\n");
	unsigned char x;
	char string[12];
	int count = 0;
	while (1) {
		if (read(fd, &x, sizeof(x)) == -1) {
			return 2;
		}
		if ((x >= 32) && (x <= 126)) {
			string[count] = x;
			count++;
			//printf("\n%c", x);
		}
		if (count == 11) {
			printf("\n%s", string);
			count = 0;
		}
		write(fd, 0, sizeof(x));
	        
	}
	printf("Written\n");
	close(fd);
	printf("Closed\n");
}
