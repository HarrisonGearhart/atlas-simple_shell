#!/bin/bash

if [[ ! -x "./hsh" ]]; then
	echo "hsh executable not found!"
	exit 1
fi

if [[ ! -x "./bin_ls.bash" ]]; then
	echo "bin_ls.bash script not found!"
	exit 1
fi

echo "Current directory: $(pwd)"

echo "Running tests..."

echo "/bin/ls" | ./hsh > output.txt

if [[ -f "expected_output.txt" ]]; then
	diff output.txt expected_output.txt
else
	echo "Expected output file 'expected_output.txt' not found!"
	exit 1
fi

valgrind --leak-check=full ./hsh < bin_ls.bash
