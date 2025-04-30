#!/bin/bash

#compile the main program
g++ main.cpp constants.cpp -o main -pthread

#compile the TCP test program
g++ tcp_test.cpp -o tcp_test -pthread

#run the main program
./main