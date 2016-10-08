#include <stdlib.h>
#include <unistd.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <string.h>  
#define BUFFER_SIZE 1024  

/*return value:  1: file exit
		-1: file no exit
*/
int file_exit(const char *filename)
{
	int retvalue;
	if (filename == NULL) {
		retvalue = -1;
		goto end;
	}
	/*judge file is read and write or no*/
	if (0 == access(filename, 2 | 4))
		retvalue = 1;
	else
		retvalue = -1;
end:
	return retvalue;
}

int file_create(const char *filename)
{
	int retvalue = -1;
	int fd = -1;
	if (filename == NULL) {
		retvalue = -1;
		goto end;
	}
	fd = open(filename, O_CREAT | O_RDWR, S_IRWXU | S_IRWXO | S_IRWXG);
	if (fd > 0)
		retvalue = fd;
	else
		retvalue = -1;
end:
	return retvalue;
}

int main(int argc, char **argv)
{
	int retvalue;
	if (argc != 2)
		return -1;
	retvalue = file_create(argv[1]);
	printf("file create : %d\n", retvalue);
}
