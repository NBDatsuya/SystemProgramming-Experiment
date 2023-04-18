/*
	cp-R Implementation
	Experiment 1-3

	Huang Xuda Samuel
	2023.4
*/
#include "../myheader.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


#define BUF_SIZE 512
#define DEFAULT_ROLE 0644
#define PATH_SIZE 1024
#define ERR_EXIT 1
#define ERR_RET -1

void recur_readdir(const char[],const char[]);
void copy_file(const char[],const char[]);
void get_abspath(char[], char[], const char[], const char[]);
void mk_newdir(const char[]);
void syserr(const char[]);

int main(int argc, char *argv[]){

    // Bad args
    if (argc != 3){
        printf("USE: COMMAND SOURCE DEST\n");
        exit(ERR_EXIT);
    }

    // Recursively read folder and copy file

    recur_readdir(argv[1], argv[2]);

}

void recur_readdir(const char source_dir[],const char dest_dir[]){
    DIR *dp;
    struct dirent *p;
    struct stat dir_stat;
    char child_dirname[PATH_SIZE];
    char temp_dest[PATH_SIZE];

    if((dp=opendir(source_dir))==NULL || chdir(source_dir)==ERR_RET)
        syserr("Cannot open");

    while(p=readdir(dp)){
        if (p->d_name[0] == '.')
		continue;
        
	    stat(p->d_name, &dir_stat);

        if(S_ISDIR(dir_stat.st_mode)){

            // It's a directory
            strcat(temp_dest, "/");
            strcat(temp_dest, p->d_name);
            puts(temp_dest);
            recur_readdir(p->d_name, temp_dest);
            continue;
        }else{

            // It's a file
            strcat(temp_dest,"/");
            strcat(temp_dest,p->d_name);
            puts(temp_dest);
            copy_file(p->d_name, temp_dest);
        }
            
    }
}
void copy_file(const char source[], const char target[]){
    int fd_read, fd_write;

    char buf[BUF_SIZE];
    int n;

    if ((fd_read = open(source, O_RDONLY)) == ERR_RET)
        errmsg("Cannot open directory");


    if ((fd_write = open(target, O_CREAT|O_WRONLY, DEFAULT_ROLE)) == ERR_RET)
        errmsg("Error occurred when creating target file");

    while ((n = read(fd_read, buf, sizeof(buf))) != 0){
        if (n == ERR_RET)
            errmsg("Runtime error");

        write(fd_write, buf, n);
    }

    close(fd_write);
    close(fd_read);
}

void get_abspath(char source[], char dest[], const char s_path[], const char d_path[]){
    char current[PATH_SIZE];
    // Get the current work dir
    getcwd(current, PATH_SIZE);

    // Enter the source path
    chdir(s_path);
    // Get the source work dir
    getcwd(source, PATH_SIZE);
    strcat(source,"/");
    
    // Back to current dir
    chdir(current);
    // Make new dir and enter into it
    mk_newdir(d_path);
    chdir(d_path);

    // Get the destination work dir
    getcwd(dest, PATH_SIZE);
    strcat(dest, "/");
}

void mk_newdir(const char path[]){
    DIR *dp;
    if((dp=opendir(path))==NULL){
        int flag_mkdirs;
        if((flag_mkdirs= mkdir(path, DEFAULT_ROLE))==ERR_RET)
            errmsg("Cannot create directory");
    }
}

void init_paths(char source){

}