CC = gcc
CCFLAGS = -Wall -lm
SRC = tema0.c
PROGRAM = tema0
 
build:
	$(CC) -o $(PROGRAM) $(SRC) $(CCFLAGS)
 
run : build
	./$(PROGRAM)
 
.PHONY : clean
clean :
	rm -f $(PROGRAM) core *~
