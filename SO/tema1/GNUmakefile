CC = gcc
C_FLAGS = -Wall
SOURCE1 = tema1.c
SOURCE2 = hash.c
EXE = tema1

all: build

build: $(SOURCE1) $(SOURCE2)
	$(CC) $(C_FLAGS) $(SOURCE1) $(SOURCE2) -o $(EXE)

.PHONY: clean

clean: $(EXE)
	rm -f $(EXE) core *~
