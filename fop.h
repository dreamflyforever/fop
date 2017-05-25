#ifndef _fop_h_
#define _fop_h_

#define print(format, ...) \
	{ \
		printf("[%s : %s : %d] ", \
		__FILE__, __func__, __LINE__); \
		printf(format, ##__VA_ARGS__); \
	}

int file_exist(const char *filename);
int file_create(const char *filename);
int file_read(void *fd_p, char *buf, int size);
int file_write(void *fd_p, char *content, int size);

#endif
