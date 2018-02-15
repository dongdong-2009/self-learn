#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int main(int argc,char **argv)
{
	DIR *dp;
	struct dirent *dirp;

	if(argc != 2){
		printf("Usage: ls directory_name\n");
		exit(1);
	}

	dp = opendir(argv[1]);
	if(dp == NULL){
		printf("Can't open %s\n",argv[1]);
		exit(1);
	}

	while((dirp = readdir(dp)) != NULL){
		printf("%s\n",dirp->d_name);
	}

	closedir(dp);

	exit(0);
}
