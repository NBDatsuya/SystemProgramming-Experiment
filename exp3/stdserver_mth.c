/*
	Standard Server
	Experiment 3-3
	
	Multi-Thread

	Huang Xuda Samuel
	2023.4
*/
#include "../myheader.h"
#include "mywebserver.h"
#include <pthread.h>

#define MAX_CLIENT 32

/// @brief Create a thread to handle request
/// @param listen_fd* the client fd
void *create_threadhandler(void*);

int main(int argc, char *argv[]){
    int tcp_socket;	// Socket
    struct sockaddr_in addr;	// Address 2 bind
    int cfd;	// Client File Descriptor
	int *cfd_pointer;
	int rv_fork;	// Parent-Child Process Identifier
	char buf[BUF_SIZE];	//Buffer
	pthread_t t_client;

	// Bad args
	if(argc < 2)
		usage("USE: CMD PORT");

	// Create end point
    tcp_socket = socket(AF_INET, SOCK_STREAM, 0);	

	// Set protocol, port and in-address
    addr.sin_family = AF_INET;
	// Using atoi() to convert ascii code into int
    addr.sin_port = htons(atoi(argv[1]));
	// Address to accept any incoming messages.
    addr.sin_addr.s_addr = INADDR_ANY;

	// Cannot bind
    if(bind(tcp_socket, 
			(const struct sockaddr *)&addr, 
			sizeof(struct sockaddr_in))==-1)
			
        errmsg("Cannot bind");

	// Ignore pipe
	signal(SIGPIPE, SIG_IGN);

	// Listen for the connection
    listen(tcp_socket, 1);

	// -----------------------------------------
	// Multi-Thread
	// -----------------------------------------
	while (1){
		cfd = accept(tcp_socket, NULL, NULL);
		*cfd_pointer = cfd;
		pthread_create(&t_client, NULL, create_threadhandler, cfd_pointer);
	}
}

void *create_threadhandler(void *listen_fd){
	int cfd = *((int *) listen_fd);
	char buf[BUF_SIZE];

	read(cfd, buf, sizeof(buf));
	handle_rq(cfd, buf);
	close(cfd);
}