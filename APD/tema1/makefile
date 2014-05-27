CC = g++
C_FLAGS = -g -Wall
OMP_FLAGS = -fopenmp

SERIAL = serial.cpp
SERIAL_OPT = serial_opt.cpp
PARALEL = paralel.cpp
PARALEL_OPT = paralel_opt.cpp

SERIAL_EXE = serial
SERIAL_OPT_EXE = serial_opt
PARALEL_EXE = paralel
PARALEL_OPT_EXE = paralel_opt

all: build_serial build_serial_opt build_paralel build_paralel_opt

build_serial:
	$(CC) $(SERIAL) -o $(SERIAL_EXE) $(C_FLAGS)
	
build_serial_opt:
	$(CC) $(SERIAL_OPT) -o $(SERIAL_OPT_EXE) $(C_FLAGS)

build_paralel:	
	$(CC) $(OMP_FLAGS) $(PARALEL) -o $(PARALEL_EXE) $(C_FLAGS)

build_paralel_opt:	
	$(CC) $(OMP_FLAGS) $(PARALEL_OPT) -o $(PARALEL_OPT_EXE) $(C_FLAGS)

.PHONY: clean
clean:
	rm -f $(SERIAL_EXE) $(SERIAL_OPT_EXE) $(PARALEL_EXE) $(PARALEL_OPT_EXE) core *~
