/*
	mywebserver.h
	Public web server handlers
	
	Huang Xuda Samuel
	2023.4
*/

#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>

#define ROOT "root/"
#define DEFAULT "index.html"

#define PAGE_400 "root/error/400.html"
#define PAGE_404 "root/error/404.html"

#define HEAD_SUC "HTTP/1.0 200 OK\r\n\r\n"
#define HEAD_200 HEAD_SUC
#define HEAD_400 "HTTP/1.0 400 Bad Request\r\n\r\n"
#define HEAD_404 "HTTP/1.0 404 Not Found\r\n\r\n"


// Function Declarations

// --------------------------
// Handlers


/// @brief Handle the request
/// @param cfd client
/// @param request the request from client
void handle_rq(int, char*);

/// @brief Handle the GET request
/// @param cfd client
/// @param request the path of requested resource
void handle_get(int, char*);

/// @brief Handle the BAD request
/// @param cfd client
void handle_brq(int);

/// @brief Handle the Not Found Event
/// @param cfd client
void handle_ntf(int);

/// @brief Handle the 200 Event
/// @param cfd client
/// @param filefd requested resource
void handle_success(int, int);


// -------------------------------
// Capsulated write functions

/// @brief Write the header
/// @param cfd 
/// @param header 
void write_header(int, char*);

/// @brief Write page by path
/// @param cfd client
/// @param page page file
void write_crpage(int, char*);

/// @brief Write page by fd
/// @param cfd client
/// @param pfd page file descriptor
void write_fdpage(int, int);


// Function Implementations

// ------------------------
// Handlers
void handle_rq(int cfd, char *request){
	// The command and path
	char cmd[512];
	char path[1024];

	// Scan info from given datagram
	sscanf(request, "%s%s", cmd, path);

	// Supports GET method only
	if(!strcmp(cmd, "GET"))
		// Handle the get method
		handle_get(cfd, path);	
	else
		// Handle the bad request
		handle_brq(cfd);
}

void handle_get(int cfd, char *path){
	int filefd;
	char dir[BUF_SIZE];
	strcpy(dir, ROOT);
	

	// Know if the resource exists

	// Default page
	if(!strcmp(path+1,"") || !strcmp(path+1,"\0"))
		strcat(dir,DEFAULT);
	else
		strcat(dir,path+1);

	if((filefd=open(dir,O_RDONLY))==-1)
		// 404: Resource not found
		handle_ntf(cfd);
	else
		// 200: Success
		handle_success(cfd, filefd);
}

void handle_brq(int cfd){

	// Write the 400 Header
	write_header(cfd, HEAD_400);

	// Send the 400 Page
	write_crpage(cfd, PAGE_400);
}

void handle_ntf(int cfd){

	// Write the 404 Header
	write_header(cfd,HEAD_404);

	// Send the 400 Page
	write_crpage(cfd,PAGE_404);
}

void handle_success(int cfd, int filefd){

	// Write the 200 Header
	write_header(cfd, HEAD_200);

	// Send the requested Page
	write_fdpage(cfd, filefd);
}


// ----------------------
// Capsulated write functions

void write_header(int cfd, char* header){
	write(cfd, header, strlen(header));
}

void write_crpage(int cfd, char* page){
	int n;
	char buf[1024];
	int fd;
	if((fd = open(page, O_RDONLY))!=-1)
		while((n=read(fd, buf, sizeof(buf)))>0)
			write(cfd, buf, n);
}
void write_fdpage(int cfd, int pfd){
	int n;
	char buf[1024];
	while((n=read(pfd, buf, sizeof(buf)))>0)
			write(cfd, buf, n);
}