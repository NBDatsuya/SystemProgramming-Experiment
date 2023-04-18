/*
	Standard Server
	Experiment 3-2
	
	Prefork

	Huang Xuda Samuel
	2023.4
*/
#include "../myheader.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "mywebserver.h"
#define MAX_CLIENT 32


// ----------------------
// Main
int main(int argc, char *argv[]){
    int tcp_socket;	// Socket
    struct sockaddr_in addr;	// Address 2 bind
    int cfd;	// Client File Descriptor
	int n,i;	// Count
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
	// Prefork
	// -----------------------------------------

	// Fork several children
	for(i = 0;i < MAX_CLIENT; i++){
		rv_fork = fork();
		if (rv_fork == 0) break;
	}

	while(1){
		cfd=accept(tcp_socket, NULL, NULL);
        n=read(cfd, buf, sizeof(buf));
        handle_rq(cfd, buf);
		
		close(cfd);
	}
}
    