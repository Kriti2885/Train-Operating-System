# Train Operating System

TOS (TrainOS) is a simple operating system for Intel x86 based PCs. The train operating system is written in C and assembly language. This operating system is able to control a real train model.
 

Credits:

This code is built on the reference implementation of a toy operating system by Professor Arno Puder for the CSC 720 class at San Francisco State University.

Implementation:

A. Implemented Shell/ command line interpreter
B. Implemented an OS for train. 
For more info on train OS : https://www.youtube.com/watch?v=PXImopYiK6o&feature=emb_logo

######################################################################IMPLEMENTATION DETAILS ###################################################################

A. Implemented Shell or Command Line Interpreter (e.g. to replace /bin/bash for simple interactions with the Linux Kernel.) from scratch in C language. The shell support following 7 commands: 

1. Execute a single command with up to four command line arguments (including command line arguments with associated flags). 
For example: ls –l cat myfile 
Bash equivalent: ls –al /usr/src/linux

2. Execute a command in background. 
For example: ls -l & 
Bash Equivalent: ls –al /usr/src/linux &

3. Redirect the standard output of a command to a file. 

For example:
ls -l > outfile
ls -l >> outfile

Bash equivalent: 
ls –al /usr/src/linux > outfile2
ls –al /usr/src/linux >>outfile2

4. Redirect the standard input of a command to come from a file. 
For example: grep disk < outfile
Bash Equivalen : grep linux < outfile2

5. Execute multiple commands connected by a single shell pipe. 
For example: ls –al 
Bash equivalent: /usr/src/linux | grep linux

6. Execute the cd and pwd commands
cd some_path
pwd

7. History 
Shell can store a history of all commands executed. This includes the ability to scroll using up and down arrow keys and rerun previously entered commands.
Bash equivalent: history
