q1/named\ pipe/  folder contains c codes for question 1(named pipe) of execution in terminal.
FILES:

P1.c
	-> c code for process 1
P2.c
	-> c code for process 2
P3.c
	-> c code for process 3

Execution steps:
This problem question is done using named pipes
1. compile and run all c files in different terminals 
	- gcc P?.c -o P?.o -lm
	- ./P?.o
	(P? indicates that which process's file you are compiling and running | ? ->(1,2,3))
