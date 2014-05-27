/*
 *	Tema 3 SO: Memorie virtuala
 *
 *	nume/prenume: 	Bojin Andrei Florian
 *	grupa/seria: 	333 CA
*/

CC = gcc
CFLAGS = -Wall -fPIC
LDLIBS= -lrt -shared

build: vmsim.o common_lin.o
	$(CC) vmsim.o common_lin.o -o libvmsim.so $(LDLIBS)

vmsim.o: vmsim.c
	$(CC) -c vmsim.c $(CFLAGS)

common_lin.o: common_lin.c
	$(CC) -c common_lin.c $(CFLAGS)

clean:
	rm -f *.o *~ libvmsim.so
