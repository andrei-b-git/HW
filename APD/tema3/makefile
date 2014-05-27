CC = mpicc
C_FLAGS = -lm -Wall
SRC = tema3.c
OUT = main

all: build

build:
	$(CC) $(SRC) -o $(OUT) $(C_FLAGS)

.PHONY: clean

clean:
	rm -f $(OUT) core *~
