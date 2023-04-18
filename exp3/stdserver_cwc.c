/*
	Standard Server
	Experiment 3-1
	
	Create child processes when connected

	Huang Xuda Samuel
	2023.4
*/
#include "../myheader.h"

#include "mywebserver.h"


// ----------------------
// Main
int main(int argc, char *argv[]){
    int tcp_socket;	// Socket
    struct sockaddr_in addr;	// Address 2 bind
    int cfd;	// Client File Descriptor
	int n;	// Count
	int rv_fork;	// Parent-Child Process Identifier
	char buf[BUF_SIZE];	//Buffer

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

	// Ignore pipe and child signals
	signal(SIGPIPE, SIG_IGN);
	signal(SIGCHLD, SIG_IGN);	// AVOID DETACHED GHOSTS!!!

	// Listen for the connection
    listen(tcp_socket, 1);


	// -----------------------------------------
	// Create child process when connected
	// -----------------------------------------

    while(1){
        cfd = accept(tcp_socket, NULL, NULL);
		if((rv_fork=fork())==-1){
			perror("Cannot fork");
			continue;
		}

		if(rv_fork==0){
			// Child Process
			n = read(cfd,buf,sizeof(buf));

			// Handle requests
			handle_rq(cfd,buf);

			// Recycling resources
			close(cfd);
			exit(0);
		}else{
			// Parent Process
			close(cfd);
		}
    }
}

// ----------------------
// Handlers

// ----------------------
// Capsulated write functions

/* 
	Both kinds of functions were first implemented here
	But soon moved to handlers.h
	for the multiple use purpose
 */