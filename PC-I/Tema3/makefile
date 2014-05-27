CC = gcc
CCFLAGS = -Wall 
SRC = tema3.c
PROGRAM = tema3

all: build run clean
 
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)
 
run: build
	./$(PROGRAM)
 
.PHONY: clean
clean:
	rm -f $(PROGRAM) core *~
