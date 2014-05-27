/*	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 323 CA
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<math.h>

#include "lib.h"
#include "lib1.h"
#include "Queue.h"

#define BITS_NO 	8

int main(int argc, char *argv[])
{
	char *filename;
	int task_index, speed, delay, filesize;
	int i, fd, timeout, res, COUNT;
	int bdp, window, FRAME_SIZE;
	char read_buffer[MSGSIZE];
	struct stat f_status;

	task_index = atoi(argv[1]);
	filename = argv[2];
	speed = atoi(argv[3]);
	delay = atoi(argv[4]);
	
	printf("[SENDER] Sender starts.\n");
	printf("[SENDER] Filename=%s, task_index=%d, speed=%d, delay=%d\n", filename, task_index, speed, delay);

	/* miliseconds for delay & megabits for speed */
	bdp = speed * delay * 1000;
	printf("[SENDER] BDP = %d b(bits).\n", bdp);

	fd = open(filename, O_RDONLY);
	fstat(fd, &f_status);

	filesize = (int) f_status.st_size;
	printf("[SENDER] File size: %d\n", filesize);

	/* miliseconds for timeout */
	if(2*delay > 1000)
		timeout = 2*delay;
	else
		timeout = 1000;
	printf("[SENDER] timeout = %d\n", timeout);	

	init(HOST, PORT1);

	if(task_index == 0 || task_index == 1) {

		msg t;
		my_pkt p;
		int SCAN, bsize;
		queue *buffer;
		buffer = (queue *) malloc(sizeof(queue));
		create_queue(buffer, sizeof(msg));
		FRAME_SIZE = MSGSIZE + 4;

		printf("[SENDER] Each frame has %d B(bytes).\n", FRAME_SIZE);
		COUNT = filesize / FRAME_SIZE;
		printf("[SENDER] Gonna send %d frames.\n", COUNT);
		/* window = number of frames in the 'network', unacked */
		window = bdp / (FRAME_SIZE * BITS_NO);
		printf("[SENDER] window = %d frames\n", window);

		/* Len in message 
			= size(msg payload)
			= amount of data in the my_pkt structure
			= size(type) + data in (pkt payload)
			= sizeof(int) + number of used bytes in (pkt payload)
		*/

		/* Gonna send filename - SEQ = -2 message */
		memset(t.payload, 0, sizeof(t.payload));
		memset(p.payload, 0, sizeof(p.payload));
	
		p.SEQ = -2;	
		memcpy(p.payload, filename, strlen(filename));
		t.len = sizeof(int) + strlen(filename) + 1;
		memcpy(t.payload, &p, t.len); 
		while(1) {
			/* send msg */
			while(1) {
				res = send_message(&t);
				if (res < 0) {
					perror("[SENDER] Send error. Resending");
					continue;
				}
				break;
			}
			printf("[SENDER] Filename sent.\n");

			/* Wait for filename ACK */	
			if (recv_message_timeout(&t, timeout) < 0) {
				perror("[SENDER] Receive error: TIMEOUT. Resending");
				continue;
			}
			p = *((my_pkt *) t.payload);
			if (p.SEQ != -2) {
				perror("[SENDER] Receive error: WRONG ACK. Resending");
				continue;
			}
			printf("[SENDER] Got reply with ACK: %d (Filename)\n", p.SEQ);
			break;
		}

		/* Gonna send filesize - SEQ = -1 message */
		memset(t.payload, 0, sizeof(t.payload));
		memset(p.payload, 0, sizeof(p.payload));
	
		p.SEQ = -1;
		memcpy(p.payload, &filesize, sizeof(int));
		t.len = sizeof(int) * 2;
		memcpy(t.payload, &p, t.len);
		while(1) {
			/* send msg */
			while(1) {
				res = send_message(&t);
				if (res < 0) {
					perror("[SENDER] Send error. Resending");
					continue;
				}
				break;
			}
			printf("[SENDER] Filesize sent.\n");
	
			/* Wait for filesize ACK */	
			if (recv_message_timeout(&t, timeout) < 0) {
				perror("[SENDER] Receive error: TIMEOUT. Resending");
				continue;
			}
	
			p = *((my_pkt *) t.payload);
			if (p.SEQ != -1) {
				perror("[SENDER] Receive error: WRONG ACK. Resending");
				continue;
			}
			printf("[SENDER] Got reply with ACK: %d (Filesize)\n", p.SEQ);
			break;
		}

		/* Send file contents - (SEQ = 0 -> COUNT) messages */
		int seq = 0;
		/* Fill the link = send window messages */
		for (i = 0; i < window; i++) {
			if((SCAN = read(fd, read_buffer, MSGSIZE - sizeof(int))) > 0) {
				memset(t.payload, 0, sizeof(t.payload));
				memset(p.payload, 0, sizeof(p.payload));
	
				p.SEQ = seq;
				memcpy(p.payload, read_buffer, SCAN);
				t.len = sizeof(int) + SCAN;
				memcpy(t.payload, &p, t.len);
				printf("[SENDER] Sending message with SEQ: %d\n", seq);

				/* put msg in buffer */
				enqueue(buffer, &t);

				/* send msg */
				while(1) {
					res = send_message(&t);
					if (res < 0) {
						perror("[SENDER] Send error. Resending");
						continue;
					}
					break;
				}
				seq++;
			}
			else
				break;
		}

		/* From now on, ack clocking, i.e., a new ack will inform 
		   us about the space released in the link */
		while(1) {
			/* wait for ACK */
			if (recv_message_timeout(&t, timeout) < 0) {
				perror("[SENDER] Receive error: TIMEOUT. Resending buffer");
				bsize = buffer->size;
				while(bsize) {
					/* clear msg from buffer */
					dequeue(buffer, &t);

					/* send msg */
					while(1) {
						res = send_message(&t);
						if (res < 0) {
							perror("[SENDER] Send error. Resending");
							continue;
						}
						break;
					}

					/* reput msg in buffer, in case it will be lost again */
					enqueue(buffer, &t);
					bsize--;				
				}
				continue;
			}
			else {
				p = *((my_pkt *) t.payload);
				printf("[SENDER] Got reply with ACK: %d\n", p.SEQ);

				/* clear succesfully sent msg from buffer */
				dequeue(buffer, &t);

				if((SCAN = read(fd, read_buffer, MSGSIZE - sizeof(int))) > 0) {
					memset(t.payload, 0, sizeof(t.payload));
					memset(p.payload, 0, sizeof(p.payload));
		
					p.SEQ = seq;
					memcpy(p.payload, read_buffer, SCAN);
					t.len = sizeof(int) + SCAN;
					memcpy(t.payload, &p, t.len);
					printf("[SENDER] Sending message with SEQ: %d\n", seq);

					/* put msg in buffer */
					enqueue(buffer, &t);

					/* send msg */
					while(1) {
						res = send_message(&t);
						if (res < 0) {
							perror("[SENDER] Send error. Resending");
							continue;
						}
						break;
					}
					seq++;
				}
				else
					if(buffer->size == 0)
						break;
			}
		}

		destroy_queue(buffer);
		free(buffer);
	}
	
	printf("[SERVER] File transfer has ended.\n");

	close(fd);
	
	return 0;
}
