import subprocess

def parse_test_case(input_lines, expected_line):
    """Parse a pair of input and expected output from the test case lines."""
    input_str = [line.strip() for line in input_lines]
    # Clean up the expected output and remove any leading spaces after ';=>'
    expected_output = expected_line.strip().split('=>')[1].strip() if '=>' in expected_line else expected_line.strip()
    return input_str, expected_output

def run_repl(input_str):
    """Function to send input to the REPL and get the output."""
    # print("Starting subprocess for main.exe...")
    process = subprocess.Popen(
        ["./main.exe"],        # Path to main.exe
        stdin=subprocess.PIPE, # Allow us to send input to the REPL
        stdout=subprocess.PIPE, # Capture the output of the REPL
        stderr=subprocess.PIPE,
        text=True               # Use text mode for input/output (to handle strings)
    )
    # Wait for the initial prompt "user (CTRL + C to exit)>"
    output = ""
    # Now send the input to the REPL
    # print(f"Sending input to REPL: {input_str}")
    for i in range(len(input_str)):
        process.stdin.write(input_str[i] + "\n")
        process.stdin.flush()
        if i != len(input_str) - 1:
            process.stdout.readline()


    # Read the output
    output_lines = []
    while True:
        output = process.stdout.readline().strip()  # Read each line from the REPL
        output = output.replace("user (CTRL + C to exit)>", "")
        # print(f"REPL Output (after sending input): {output}")
        if output:  # If the output is not empty
            output_lines.append(output)
        break
        # Assuming we can stop after the REPL responds once, exit the loop
        if "user (CTRL + C to exit)>" in output.strip():  # Detect the next prompt
            print("Detected next prompt.")
            break

    # Join all output lines into a single string for easier comparison
    output = "\n".join(output_lines).strip()
    # print(f"Received REPL Output: {output}")
    return output

def run_tests(test_file):
    """Read test cases from the file and run them."""
    with open(test_file, 'r') as file:
        lines = file.readlines()

    # Iterate through the lines and pair input with expected output
    input_lines = []
    for i in range(0, len(lines)):
        input_line = lines[i].strip()
        if i + 1 < len(lines):
            expected_line = lines[i + 1].strip()
        else:
            continue  # Skip if there's no corresponding expected output

        # Skip comments or empty lines
        if input_line.startswith(';;') or not input_line:
            if(input_line.startswith(';;')):
                print()
                print(input_line.replace(";;", ""))
                print("==========================================")
            continue
        if expected_line.startswith(';;') or not expected_line:
            continue
        input_lines.append(input_line)

        # Check if this is a valid test case (input should not start with ;=>)
        if expected_line.startswith(";=>"):
            # print(f"Testing input: {input_line}")
            # print("input lines: ", input_lines)
            input_str, expected_output = parse_test_case(input_lines, expected_line)
            
            # Call the REPL function and get the output
            actual_output = run_repl(input_str)

            # Compare the actual output with the expected output
            # print(f"Comparing actual output to expected: {expected_output}")
            print()
            print("Input: ")
            for line in input_lines:
                print(line)
            if actual_output == expected_output or (expected_output == "()" and actual_output == "nil") or (expected_output == "nil" and actual_output == "()"):
                print(f"PASS: {actual_output}")
            else:
                print(f"FAIL: Expected '{expected_output}', but got '{actual_output}'")
            input_lines = []

if __name__ == "__main__":
    # Path to the test file
    test_file_path = 'test_cases.txt'  # Replace with your file name or path
    run_tests(test_file_path)
