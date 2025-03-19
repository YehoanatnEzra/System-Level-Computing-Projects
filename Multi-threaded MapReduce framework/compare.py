def compare_files(output_f, expected_f):
	with open(output_f, 'r') as out_f, open(expected_f, 'r') as exp_f:
		out_lines = [line.strip() for line in out_f.readlines()]
		exp_lines = [line.strip() for line in exp_f.readlines()]

	if out_lines == exp_lines:
		print( "Test passed!")
	else:
		print("Test failed!")
		for i, (out, exp) in enumerate(zip(out_lines, exp_lines), 1):
			if out != exp:
				print(f" Line {i}: Expected '{exp}', but got '{out}'")

compare_files("./Multi-threaded MapReduce framework/test1_output",
              "./Multi-threaded MapReduce framework/test1-1_thread_1 - expected.txt")
