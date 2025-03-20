import os


def compare_files(output_f, expected_f):
    if not os.path.exists(output_f):
        print(f"❌ Error: Output file '{output_f}' not found.")
        return
    if not os.path.exists(expected_f):
        print(f"❌ Error: Expected file '{expected_f}' not found.")
        return

    with open(output_f, 'r', encoding="utf-8", errors="ignore") as out_f, \
            open(expected_f, 'r', encoding="utf-8", errors="ignore") as exp_f:

        out_lines = [line.strip() for line in out_f.readlines()]
        exp_lines = [line.strip() for line in exp_f.readlines()]

    if out_lines == exp_lines:
        print("✅ Test Passed! The output matches the expected output.")
    else:
        print("❌ Test Failed! Mismatch detected.")
        for i, (out, exp) in enumerate(zip(out_lines, exp_lines), 1):
            if out != exp:
                print(f"🔸 Line {i}: Expected '{exp}', but got '{out}'")

compare_files("Multi-threaded MapReduce framework/test1_output",
              "Multi-threaded MapReduce framework/test1-1_thread_1_expected.txt")
