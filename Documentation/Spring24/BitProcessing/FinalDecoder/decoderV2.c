#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>

char* btoB(unsigned char* dst, unsigned char* b, int n) {
        unsigned char byte = 0;
        int count = 0;
        for (int i=0; i<n; i++) {
                for (int j=1; j<=8; j++) {
                        byte = byte | (b[i*8+j] << (8-j));
                }
                dst[i] = byte;
                //printf("\ncurrent char: %u", (int) byte);
                byte = 0;
        }
        return 0;
}

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
	
	unsigned char syncWord[] = {1,1,0,1,0,0,1,1,1,0,0,1,0,0,0,1};
	int bufLength = 100;
	unsigned char buffer[bufLength];
	
	int pLength = 8+1;
	unsigned char bits[pLength*8];
	unsigned char* data = malloc(pLength);
	
	int count = 0;
	int STATE = 0;
	//STATE = 0 means idle, STATE = 1 means looking, STATE = 2 means collecting
	
	while (1) {
	        if (read(fd, &x, sizeof(x)) == -1) {
		        return 2;
	        }
	        if ((x == 0) && (STATE == 0)) {
	                memset(buffer,0,bufLength);
	                STATE = 1;
	                //printf("buffer start");
	        }
	        if (STATE == 1) {
	        	buffer[count] = x;
	                if (memmem(buffer, bufLength, syncWord, sizeof(syncWord)) != NULL) {
	                        STATE = 2;
	                        count = 0;
	                        //printf("\nPacket Detected!");
	                        memset(buffer, 0, bufLength);
	                } else if (count < bufLength) {
	                        count++;
	                } else {
	                        count = 0;
	                        memset(buffer,0,bufLength);
	                        STATE = 0;
	                        //printf("\nJust Noise");
	                }
	        }
	        if (STATE == 2) {
	                bits[count] = x;
	                if (count < pLength*8) {
	                        count++;
	                } else {
	                        count = 0;
	                        STATE = 0;
	                        btoB(data, bits, pLength);
	                        printf("\nData Processed: %s", data);
	                }     
	        }
	        write(fd, 0, sizeof(x));
	}
}
