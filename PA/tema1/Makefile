CC = g++
CCFLAGS = -g -Wall 
SRC1 = p1.cpp
PROGRAM1 = p1
SRC2 = p2.cpp
PROGRAM2 = p2
SRC3 = pb.cpp
PROGRAM3 = pb
 
build:
	$(CC) $(SRC1) -o $(PROGRAM1) $(CCFLAGS)
	$(CC) $(SRC2) -o $(PROGRAM2) $(CCFLAGS)
	$(CC) $(SRC3) -o $(PROGRAM3) $(CCFLAGS)
run-p1:
	./$(PROGRAM1)
run-p2:
	./$(PROGRAM2)
run-pb:
	./$(PROGRAM3)
.PHONY: clean
clean:
	rm -f $(PROGRAM1) core *~
	rm -f $(PROGRAM2) core *~
	rm -f $(PROGRAM3) core *~
