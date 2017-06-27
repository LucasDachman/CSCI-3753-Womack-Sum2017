#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <linux/types.h>
#include <unistd.h>
#include <fcntl.h>
#define BUFFER_SIZE 1024

void read_buff();
void write_buff(char*);

const char *FILE_NAME = "/dev/char_device";
char buffer[BUFFER_SIZE];

int main(int argc, char *argv[]) {
	
	char command;
	printf("commands: (r)ead, (w)rite <string>, (q)uit\n");
	while(1) {
		char input[BUFFER_SIZE+6];
		char mesg[BUFFER_SIZE];
		printf(">> ");
		scanf("%s", input );
		command = input[0];
		switch(command) {
			case 'r' :
				read_buff();
				break;
			case 'w' :
				//mesg = strchr(&command, ' ');
				scanf("%s", mesg);
				write_buff(mesg);
				break;
			case 'q' :
				exit(0);
				break;
			default :
				printf("unknown command\n");
		}
	}
	
}

void read_buff() {
	// try to open file
	int fd = open(FILE_NAME, O_RDONLY);
	if( fd < 0 ) {
		printf("Could not open file.\n");	
		return;
	}
	int status = read(fd, buffer, BUFFER_SIZE);
	printf("output: %s", buffer);
	printf("\n");
	if( status < 0)
		 printf("read status: %d, errno: %s\n", status, strerror(errno));
	close(fd);
}

void write_buff(char *str) {
	int fd  = open(FILE_NAME, O_WRONLY);
	if( fd < 0 ) {
		printf("Could not open file.\n");
		return;
	}
	int status = write(fd, str, sizeof(str));
	if( status < 0) 
		printf("write status: %d, errno: %s\n", status, strerror(errno));
	close(fd);
}
