def compare_files(output_f, expected_f):
	with open(output_f, 'r') as out_f, open(expected_f, 'r') as exp_f:
		out_f = open(output_file, 'r')
		exp_f = open(expected_file, 'r')
	if out_f== exp_f:
		print( "Test passed!")
	else:
		print("Test failed!")

compare_files("Multi-threaded MapReduce framework/test1_output", "Multi-threaded MapReduce framework/test1-1_thread_1_expected.txt")
