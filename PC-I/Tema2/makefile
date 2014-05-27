CC = gcc
CCFLAGS = -Wall -lm
SRC1 = tema2.c
SRC2 = automat.c
PROGRAM = tema2

all: build run clean
 
build:
	$(CC) -o $(PROGRAM) $(SRC1) $(SRC2) $(CCFLAGS)
 
run: build
	./$(PROGRAM)
 
.PHONY: clean
clean:
	rm -f $(PROGRAM) core *~
