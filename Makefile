# Variables
CC = gcc #The compiler to use
CFLAGS = -Wall -g -lm #Compilation option
TARGET = main #Name of the executable file
SRCS = $(wildcard *.c) #Recover all files ending with .c

#Compilation
all:
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

#Clean the folder (delete executable)
clean:
	rm -f $(TARGET)

#Recompile with delete files
re: clean all