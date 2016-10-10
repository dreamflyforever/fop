env = Environment(
	CC = 'clang',
	CFLAGS = ['-g', '-Wall', '-I.']
)

test_case = env.Program("test_case", ['main.c', 'json_op.c', 'fop.c', 'cJSON.c', 'itoa.c'], LIBS='m')
