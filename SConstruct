env = Environment(
	CC = 'clang',
	CFLAGS = ['-g', '-Wall', '-I.']
)

obj_cJSON = env.Object('cJSON.c')
obj_itoa = env.Object('itoa.c')
obj_json_op = env.Object('json_op.c')
obj_fop = env.Object('fop.c');

json_test_case = env.Program("json_test_case",
				['json_op_test_case.c',
				obj_json_op,
				obj_cJSON,
				obj_itoa],
				LIBS='m')

test_case = env.Program("test_case",
			['main.c', obj_json_op, obj_fop,
			obj_cJSON, obj_itoa], LIBS='m')
