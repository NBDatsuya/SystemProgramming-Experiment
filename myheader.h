/*
    myheader.h
    Frequently used lib function includings
    And some f-used diy functions
    Huang Xuda Samuel 
    2023.3
*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define BUF_SIZE 1024

void errmsg(char*);
void usage(char*);


void errmsg(char* msg){
    perror(msg);
	exit(1);
}

void usage(char* msg){
    puts(msg);
	exit(1);
}