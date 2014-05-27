CC = gcc
CCFLAGS = -Wall -lm
SRC = tema1.c
PROGRAM = tema1

all: build run clean
 
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)
 
run: build
	./$(PROGRAM)
 
.PHONY: clean
clean:
	rm -f $(PROGRAM) core *~
