#include <stdlib.h>
#include <stdio.h>  
#include <string.h>
#include <unistd.h>  
#include <fcntl.h>
#include "cJSON.h"
#include "json_op.h"

int low_output_cb(int arg, char *s, int size)
{
	int retvalue = 1;
	if (s == NULL) {
		retvalue = -1;
		goto end;
	}
	print("[arg : size : s] [%d : %d : %s]\n", arg, size, s);
end:
	return retvalue;
}

int high_output_cb(void *contex, char *a, char *b, char *c)
{
	int retvalue = 1;
	if ((a== NULL) || (b == NULL) || (c == NULL)) {
		retvalue = -1;
		goto end;
	}
	print("[a:b:c] [%s : %s : %s]\n", a, b, c);
end:
	return retvalue;
}

int low_input_cb(int arg, char *s, int size)
{
	int retvalue = 1;
	if (s == NULL) {
		retvalue = -1;
		goto end;
	}

	int fd = open("./config", O_CREAT | O_RDWR, S_IRWXU | S_IRWXO | S_IRWXG);
	if (fd <= 0) {
		retvalue = -1;
		goto end;
	}
	read(fd, s, size);

	print("[arg : size : s] [%d : %d : %s]\n", arg, size, s);
end:
	return retvalue;
}

int op_high_input_func(struct op *o)
{
	int i;
	int retvalue = 1;
	if (o == NULL) {
		print("error\n");
		retvalue = -1;
		goto end;
	}
	char str[3] = {0};
	for (i = 0; i < 2000; i++) {
		itoa(i, str, 10);
		retvalue = op_high_input(i, o, str, "b", str);
		if (retvalue == -1) {
			goto end;
		}
	}
end:
	return retvalue;
}

int main()
{
	struct op *o;
	op_init(&o, 0, NULL);
	op_reg_low_output(o, low_output_cb);
	op_reg_high_output(o, high_output_cb);
	op_reg_low_input(o, low_input_cb);

	op_low_input(o);
	int i;
	for (i = 0; i < 2000; i++)
		op_high_output(o, i);

	op_high_input_func(o);
	op_low_output(o);

	op_delete(&o);
	return 0;
}
