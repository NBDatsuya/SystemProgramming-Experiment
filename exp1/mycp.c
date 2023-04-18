/*
	cp Implementation
	Experiment 1-2

	Huang Xuda Samuel
	2023.4
*/
#include "../myheader.h"

#include <sys/types.h>
#include <sys/stat.h>

#define BUF_SIZE 512
#define DEFAULT_ROLE 0644

int main(int argc, char *argv[]){
    int fd_read, fd_write;

    char buf[BUF_SIZE];
    int n;

    if (argc < 3){
        printf("USE: COMMAND SOURCE TARGET\n");
        exit(1);
    }

    if ((fd_read = open(argv[1], O_RDONLY)) == -1){
        perror("Cannot open file");
        exit(1);
    }

    if ((fd_write = open(argv[2], O_CREAT|O_WRONLY, DEFAULT_ROLE)) == -1){
        perror("Error occurred when creating target file");
        exit(1);
    }

    while ((n = read(fd_read, buf, sizeof(buf))) != 0){
        if (n == -1){
            perror("Runtime error");
            exit(1);
        }
        write(fd_write, buf, n);
    }

    close(fd_write);
    close(fd_read);
    printf("\n");
}
