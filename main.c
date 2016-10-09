#include <stdlib.h>
#include <unistd.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <errno.h>  
#include <string.h>
#include "cJSON.h"
#include "fop.h"

#define debug 0
typedef void (*func)(void **obj, char *a, char *b, char *c);
extern char *itoa(int num,char *str,int radix);

#define BUFFER_SIZE 1024  
#define print(format, ...) \
	{ \
		printf("[%s : %s : %d] ", \
		__FILE__, __func__, __LINE__); \
		printf(format, ##__VA_ARGS__); \
	}

void music_list_output(void **obj, char *a, char *b, char *c)
{
	if ((a == NULL) || (b == NULL) || (c == NULL)) {
		print("a || b || c = 0\n");
		return ;
	}
	print("%s %s %s\n", a, b, c);
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

/*music insert operation should be must after this function*/
int parse_cjson_high_output(char *buf, int key, func output,
		void **obj)
{
	int retvalue = 1;
	if (buf == NULL) {
		retvalue = -1;
		goto end;
	}

	char str[25] = {0};
	itoa(key, str, 10);
	cJSON *root = cJSON_Parse(buf);
	cJSON *music = cJSON_GetObjectItem(root, str);
	if (music == NULL) {
		retvalue = -1;
#if debug
		print("key is no in json\n");
#endif
		goto end;
	}

	char *title = cJSON_GetObjectItem(music, "title")->valuestring;
	char *artist = cJSON_GetObjectItem(music, "artist")->valuestring;
	char *url = cJSON_GetObjectItem(music, "url")->valuestring;
	if ((NULL == title) || (NULL == artist) || (NULL == url)) {
		retvalue = -1;
		print("key is no in json\n");
		goto end;
	}
	output(obj, title, artist, url);
	print("%s %s %s\n", title, artist, url);
	free(title);
	free(artist);
	free(url);
end:
	return retvalue;
}

int parse_cjson_input()
{
	int retvalue = -1;
	return retvalue;
}

int cjson_write_file_test(int fd)
{
	int i;
	cJSON *root;
	cjson_music_node_init(&root);
	for (i = 0; i < 1000; i++) {
		;
		//cjson_music_node_join(i, root, "a", "b", "c");
	}
	cjson_music_node_join(0, root, "a", "b", "c");
	cjson_music_node_join(1, root, "d", "e", "f");
	cjson_music_node_join(2, root, "g", "h", "i");
	cjson_music_node_join(3, root, "j", "k", "l");
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
	cjson_write_file_test(fd);

#if 1
	char buf[1024 * 100] = {0};
	file_read(fd, buf, 1024 * 100);
	//printf("%s", buf);
	int i;
	for (i = 0; i < 1000; i++) {
		parse_cjson_high_output(buf, i, music_list_output, NULL);
		//music_list_insert(node);
	}
#endif
end:
	return retvalue;
}
