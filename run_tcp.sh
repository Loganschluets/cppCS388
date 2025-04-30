#!/bin/bash

# Compile the TCP test program
echo "Compiling tcp_test.cpp..."
g++ tcp_test.cpp -o tcp_test -pthread

# Check if compilation was successful
if [ $? -eq 0 ]; then
    echo "Compilation successful!"
    echo "Running TCP test program..."
    echo "Press Ctrl+C to stop the test"
    ./tcp_test
else
    echo "Compilation failed!"
    exit 1
fi 