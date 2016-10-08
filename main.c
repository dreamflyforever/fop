#include <stdlib.h>
#include <unistd.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <string.h>  
#define BUFFER_SIZE 1024  
#define print(format, ...) \
	{ \
		printf("[%s : %s : %d] ", \
		__FILE__, __func__, __LINE__); \
		printf(format, ##__VA_ARGS__); \
	}

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

/*
  notice: everytimes call this function will from the file starting position to
  write.
*/
int file_write(int fd, char *content)
{
	int retvalue = 1;
	int size;
	if ((content == NULL) | (fd <= 0)) {
		print("error\n\n");
		retvalue = -1;
		goto end;
	}

	/*make sure from the starting position to write*/
	lseek(fd, 0, SEEK_SET);
	size = write(fd, content, strlen(content));
	if (size <= 0) {
		print("write error\n");
		retvalue = -1;
		goto end;
	}
end:
	return retvalue;
}

/*
  notice: everytimes call this function will from the file starting position to
  read.
*/
int file_read(int fd, char *buf, int size)
{
	int retvalue = 1;
	if ((buf == NULL) | (fd <= 0) | (size <= 0)) {
		print("error\n\n");
		retvalue = -1;
		goto end;
	}
	/*make sure from the starting position to read*/
	lseek(fd, 0, SEEK_SET);
	size = read(fd, buf, size);
	if (size <= 0) {
		print("read error\n");
		retvalue = -1;
		goto end;
	}
end:
	return retvalue;
}

int main(int argc, char **argv)
{
	int retvalue = -1;
	int fd;
	int size;
	if (argc != 2) {
		print("usage: %s file\n", argv[0]);
		return -1;
	}
	fd = file_create(argv[1]);
	if (fd == -1) {
		retvalue = -1;
		goto end;
	}
	file_write(fd, "hell world, my friend\n");
	lseek(fd, 0, SEEK_SET);
	char buf[2048] = {0};
	file_read(fd, buf, 2048);
	printf("%s", buf);
end:
	return retvalue;
}
