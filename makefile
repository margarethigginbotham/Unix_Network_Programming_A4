#
# PROGRAM:    assignment4.cc
# PROGRAMMER: Margaret Higginbotham
# LOGON ID:   Z1793581
# DATE DUE:   March 9, 2018
#

# Complier Variables
CCFLAGS = -ansi -Wall -std=c++11

# Rule to link object code files to create executable file
assignment4.exe: assignment4.cc
	g++ $(CCFLAGS) assignment4.cc -o assignment4.exe

# Pseudo-target to remove object code and executable files
clean:
	-rm assignment4.exe
