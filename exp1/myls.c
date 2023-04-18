#include <stdio.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[])
{
	DIR *dp;
	struct dirent *p;
	struct stat buf;

	if (argc > 1)
		dp = opendir(argv[1]);
	else
		dp = opendir(".");

	if (dp == NULL)
	{
		perror("Cannot open");
		exit(1);
	}

	if (argc > 1 && chdir(argv[1]) == -1)
	{
		perror("Cannot open");
		exit(1);
	}

	while ((p = readdir(dp)))
		if(p->d_name[0]!='.') 
			printf("%s  ", p->d_name);

	printf("\n");
	closedir(dp);
}
