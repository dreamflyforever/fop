#ifndef _json_op_h_
#define _json_op_h_

#define BUF_SIZE 1024 * 4
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
int op_high_input(int num, cJSON *root, char *title, char *artist, char *url);
int op_low_input(struct op *obj);
int op_delete(struct op **obj);

#endif
