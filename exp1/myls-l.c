/*
	ls-l Implementation
	Experiment 1-1

	Huang Xuda Samuel
	2023.4
*/
#include "../myheader.h"

#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>

//--------------------------
// Function Declarations

// The main function
int main(int, char **);


/// @brief Parse mode code into mode string
/// @param mode mode code
/// @param mode_str mode string
void mode2str(int, char []);

//--------------------------
// Function Implementations
int main(int argc, char *argv[]){
	DIR *dp;
	struct dirent *p;
	struct stat buf;

	char mode_str[11];

	if(argc > 2)
		usage("USE: COMMAND [-dir]");

	if (argc = 2)
		dp = opendir(argv[1]);
	else
		dp = opendir(".");

	if (dp == NULL)
		errmsg("Cannot open");

	if (argc > 1 && chdir(argv[1]) == -1)
		errmsg("Cannot open");

	
	printf("total 0\n");

	while ((p = readdir(dp))){
		if (p->d_name[0] == '.')
			continue;

		stat(p->d_name, &buf);
		mode2str(buf.st_mode, mode_str);

		printf("%s %d %s %s %d %.*s %s\n", 
				mode_str, 
				buf.st_nlink,
				getpwuid(buf.st_uid)->pw_name, 
				getgrgid(buf.st_gid)->gr_name,
				buf.st_size, 
				12, 
				4 + ctime((const time_t *)&(buf.st_mtime)), 
				p->d_name);

	}

	closedir(dp);
}

void mode2str(int mode, char mode_str[]){
	strcpy(mode_str, "----------");

	if (S_ISDIR(mode)) mode_str[0] = 'd';

	if (mode & 0400) mode_str[1] = 'r';
	if (mode & 0200) mode_str[2] = 'w';
	if (mode & 0100) mode_str[3] = 'x';
	if (mode & 0040) mode_str[4] = 'r';
	if (mode & 0020) mode_str[5] = 'w';
	if (mode & 0010) mode_str[6] = 'x';
	if (mode & 0004) mode_str[7] = 'r';
	if (mode & 0002) mode_str[8] = 'w';
	if (mode & 0001) mode_str[9] = 'x';
}
