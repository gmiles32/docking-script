# Setup a variable to reference the desired compiler
CC = gcc

#Setup a variable to hold desired compiler flags
#Removed -std=c99 flag because getopt would not work
CFLAGS = -Wall -Wextra -Wpointer-arith -Wstrict-prototypes -g

#Declare the build
.PHONY: build test clean

#Build dependendents
build: testLigand

testligand: testligand.o
	$(CC) $(CFLAGS) $^ -o $@

clean: 
	rm -f *.0 *.d a.out testLigand 