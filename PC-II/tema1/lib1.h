#ifndef LIB1
#define LIB1

#include "lib.h"

#define PKTSIZE		MSGSIZE - 4

typedef struct {
	int SEQ;
	char payload[PKTSIZE];	
} my_pkt;

#endif

