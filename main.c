#include <stdlib.h>
#include <unistd.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <string.h>
#include "cJSON.h"

#define debug 0

extern char *itoa(int num,char *str,int radix);

#define BUFFER_SIZE 1024  
#define print(format, ...) \
	{ \
		printf("[%s : %s : %d] ", \
		__FILE__, __func__, __LINE__); \
		printf(format, ##__VA_ARGS__); \
	}

int cjson_music_node_init(cJSON **root)
{
	*root=cJSON_CreateObject();
	return 0;
}

int cjson_music_node_delete(cJSON **root)
{
	 cJSON_Delete(*root);
	 return 0;
}

int cjson_music_node_join(int num, cJSON * root, char *title, char *artist, char *url)
{
	int retvalue = 1;
	if ((title == NULL) | (artist == NULL) | (url == NULL) |
		(root == NULL)) {
		retvalue = -1;
		goto end;
	}

	cJSON *fmt;
	char str[25] = {0};
	itoa(num, str, 10);
	cJSON_AddItemToObject(root, str, fmt=cJSON_CreateObject());
	cJSON_AddStringToObject(fmt, "title", title);
	cJSON_AddStringToObject(fmt, "artist", artist);
	cJSON_AddStringToObject(fmt, "url", url);
end:
	return retvalue;
}

/*return value:  1: file exit
		-1: file no exit
*/
int file_exist(const char *filename)
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
int file_write(int fd, char *content, int size)
{
	int retvalue = 1;
	if ((content == NULL) | (fd <= 0)) {
		print("error\n\n");
		retvalue = -1;
		goto end;
	}

	/*make sure from the starting position to write*/
	lseek(fd, 0, SEEK_SET);
	size = write(fd, content, size);
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

int cjson_music_node_print(int fd, cJSON *root)
{
	int retvalue = 1;
	int len;
	if (root == NULL) {
		retvalue = -1;
		goto end;
	}
	char *s = cJSON_Print(root);
#if debug
	printf("%s\n", s);
#endif
	len = strlen(s);
	file_write(fd, s, len);
	free(s);
end:
	return retvalue;
}

int cjson_op_test(int fd)
{
	int i;
	cJSON *root;
	cjson_music_node_init(&root);
	for (i = 0; i < 1000; i++) {
		cjson_music_node_join(i, root, "a", "b", "c");
	}
	cjson_music_node_print(fd, root);
	cjson_music_node_delete(&root);
	return 0;
}


int main(int argc, char **argv)
{
	int retvalue = -1;
	int fd;
	if (argc != 2) {
		print("usage: %s file\n", argv[0]);
		return -1;
	}
	fd = file_create(argv[1]);
	if (fd == -1) {
		retvalue = -1;
		goto end;
	}
	cjson_op_test(fd);

#if debug
	lseek(fd, 0, SEEK_SET);
	char buf[1024 * 100] = {0};
	file_read(fd, buf, 1024 * 100);
	printf("%s", buf);
#endif
end:
	return retvalue;
}
