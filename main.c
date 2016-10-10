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
#include "json_op.h"

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

/*user callback*/
int low_output_cb(int arg, char *s, int size)
{
	int retvalue = 1;
	if (s == NULL) {
		retvalue = -1;
		goto end;
	}
	print("%s [%d]\n", s, size);
	file_write(arg, s, size);
end:
	return retvalue;
}

/*user callback*/
int high_output_cb(char *a, char *b, char *c)
{
	int retvalue = 1;
	if ((a == NULL) || (b == NULL) || (c == NULL)) {
		retvalue = -1;
		goto end;
	}
	print("[%s] [%s] [%s]\n", a, b, c);
end:
	return retvalue;
}

/*user callback*/
int low_input_cb(int arg, char *s, int size)
{
	int retvalue = 1;

	size = file_read(arg, s, size);
	if (size == -1) {
		retvalue = -1;
		goto end;
	}
end:
	return retvalue;
}

int op_reg_low_output(struct op *o, int (*low_output)
					(int arg,
					char *s,
					int size))
{
	int retvalue = 1;
	if (o == NULL) {
		retvalue = -1;
		goto end;
	}
	o->low_output = low_output;
end:
	return retvalue;
}

int op_reg_high_output(struct op *o, int (*high_output)
					(char *a,
					char *b,
					char *c))
{
	int retvalue = 1;
	if (o == NULL) {
		retvalue = -1;
		goto end;
	}
	o->high_output = high_output;
end:
	return retvalue;
}

int op_reg_low_input(struct op *o, int (*low_input)
					(int arg,
					char *s,
					int size))
{
	int retvalue = 1;
	if (o == NULL) {
		retvalue = -1;
		goto end;
	}
	o->low_input = low_input;
end:
	return retvalue;
}

int op_init(struct op **obj, int arg)
{
	int retvalue = 1;
	*obj = (struct op *)malloc(sizeof(struct op));
	if ((*obj) == NULL) {
		print("error\n");
		retvalue = -1;
		goto end;
	}

	(*obj)->root = cJSON_CreateObject();
	if ((*obj) == NULL) {
		print("error\n");
		retvalue = -1;
		goto end;
	}

	(*obj)->arg = arg;
	op_reg_low_output((*obj), low_output_cb);
	op_reg_high_output((*obj), high_output_cb);
	op_reg_low_input((*obj), low_input_cb);
end:
	return retvalue;
}

int op_delete(struct op **obj)
{
	 cJSON_Delete((*obj)->root);
	 free(obj);
	 return 0;
}

int cjson_music_node_delete(cJSON **root)
{
	 cJSON_Delete(*root);
	 return 0;
}

int op_high_input(int num, cJSON * root, char *title, char *artist, char *url)
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

int op_low_output(struct op *o)
{
	int retvalue = 1;
	int len;
	if ((o == NULL) || (o->root == NULL)) {
		retvalue = -1;
		goto end;
	}
	char *s = cJSON_Print(o->root);
#if debug
	printf("%s\n", s);
#endif
	len = strlen(s);
	//file_write(fd, s, len);
	o->low_output(o->arg, s, len);
	free(s);
end:
	return retvalue;
}

/*music insert operation should be must after this function*/
int op_high_output(struct op *o, int key)
{
	int retvalue = 1;
	int size;
	if ((o->buf == NULL) || (o == NULL)) {
		retvalue = -1;
		print("\n");
		goto end;
	}
	size = strlen(o->buf);
	if (size == 0) {
		retvalue = -1;
		print("file is no content\n");
		goto end;
	}

	char str[25] = {0};
	itoa(key, str, 10);
	cJSON *root = cJSON_Parse(o->buf);
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
	o->high_output(title, artist, url);
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

#if 1
int node_get(struct op *o)
{
	op_high_input(0, o->root, "a", "b", "c");
	op_high_input(1, o->root, "d", "e", "f");
	op_high_input(2, o->root, "g", "h", "i");
	op_high_input(3, o->root, "j", "k", "l");
	return 0;
}
#endif

int op_low_input(struct op *obj)
{
	int retvalue = 1;
	if (obj == NULL) {
		retvalue = -1;
		goto end;
	}
	memset(obj->buf, 0, BUF_SIZE);
	retvalue = obj->low_input(obj->arg, obj->buf, BUF_SIZE);
end:
	return retvalue;
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
	struct op *o_obj;
	op_init(&o_obj, fd);

	node_get(o_obj);
	op_low_output(o_obj);

	retvalue = op_low_input(o_obj);
	if (retvalue == -1) {
		goto end;
	}
	int i;
	for (i = 0; i < 1000; i++) {
		op_high_output(o_obj, i);
	}
end:
	return retvalue;
}
