CC = gcc
CFLAGS = -Wall -g
LINK = -lpthread

build: libscheduler.so

libscheduler.so: so_scheduler.o vector.o
	$(CC) $^ -shared -o $@ $(CFLAGS)

so_scheduler.o: so_scheduler.c
	$(CC) $^ -fPIC -c -o $@ $(CFLAGS) $(LINK)

vector.o: vector.c
	$(CC) $^ -fPIC -c -o $@ $(CFLAGS) $(LINK)

clean:
	rm -rf *.o *~ libscheduler.so
