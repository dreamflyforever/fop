#include <stdlib.h>
#include <stdio.h>  
#include <string.h>
#include "cJSON.h"
#include "json_op.h"

#define debug 0

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

end:
	return retvalue;
}

int op_delete(struct op **obj)
{
	 cJSON_Delete((*obj)->root);
	 free((*obj));
	 return 0;
}

int op_high_input(int num, struct op *o, char *title, char *artist, char *url)
{
	int retvalue = 1;
	if ((title == NULL) | (artist == NULL) | (url == NULL) |
		(o->root == NULL)) {
		retvalue = -1;
		goto end;
	}

	cJSON *fmt;
	char str[25] = {0};
	itoa(num, str, 10);
	cJSON_AddItemToObject(o->root, str, fmt=cJSON_CreateObject());
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
	print("\n");
	cJSON *root = cJSON_Parse(o->buf);
	if (root == NULL) {
		print("json format buf error, maybe increase BUF_SIZE\n");
		retvalue = -1;
		goto end;
	}
	cJSON *music = cJSON_GetObjectItem(root, str);
	if (music == NULL) {
		retvalue = -1;
#if debug
		print("key is no in json\n");
#endif
		goto end_free;
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

end_free:
	cJSON_Delete(root);
end:
	return retvalue;
}

int op_low_input(struct op *obj)
{
	int retvalue = 1;
	if ((obj == NULL) || (obj->buf == NULL)) {
		retvalue = -1;
		goto end;
	}

	memset(obj->buf, 0, BUF_SIZE);
	retvalue = obj->low_input(obj->arg, obj->buf, BUF_SIZE);
end:
	return retvalue;
}