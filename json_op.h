#ifndef _json_op_h_
#define _json_op_h_

typedef void (*func)(void **obj, char *a, char *b, char *c);
extern char *itoa(int num,char *str,int radix);

#define print(format, ...) \
	{ \
		printf("[%s : %s : %d] ", \
		__FILE__, __func__, __LINE__); \
		printf(format, ##__VA_ARGS__); \
	}

#define BUF_SIZE 1024 * 100
struct op {
	int arg;
	cJSON *root;
	char buf[BUF_SIZE];
	int (*low_output)(int arg, char *s, int size);
	int (*high_output)(char *a, char *b, char *c);
	int (*low_input)(int arg, char *s, int size);
};

int op_reg_low_output(struct op *o, int (*low_output)
					(int arg,
					char *s,
					int size));

int op_reg_high_output(struct op *o, int (*high_output)
					(char *a,
					char *b,
					char *c));

int op_reg_low_input(struct op *o, int (*low_input)
					(int arg,
					char *s,
					int size));

int op_init(struct op **obj, int arg);
int op_high_output(struct op *obj, int key);
int op_low_output(struct op *obj);
int op_high_input(int num, struct op *obj, char *title, char *artist, char *url);
int op_low_input(struct op *obj);
int op_delete(struct op **obj);

#endif
