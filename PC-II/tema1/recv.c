/*	nume: Bojin
 *	prenume: Andrei Florian
 *	grupa/seria: 323 CA
*/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include "lib.h"
#include "lib1.h"

int main(int argc, char *argv[])
{
	int task_index;

	task_index = atoi(argv[1]);	
	printf("[RECEIVER] Receiver starts.\n");
	printf("[RECEIVER] Task index=%d\n", task_index);
		
	init(HOST, PORT2);

	if(task_index == 0 || task_index == 1) {

		msg t;
		my_pkt p;
		char filename[MSGSIZE], filename1[MSGSIZE];
		int filesize, fd, read_so_far, res;

		/* Wait for filename */	
		memset(t.payload, 0, sizeof(t.payload));
		if (recv_message(&t) < 0) {
			perror("[RECEIVER] Received msg error");
			return -1;
		}
	
		p = *((my_pkt*) t.payload);
		if (p.SEQ != -2) {
			perror("[RECEIVER] Received WRONG SEQ");
			return -1;
		}
	
		/* Extract message SEQ */
		printf("[RECEIVER] Message SEQ: %d (Filename)\n", p.SEQ);
		/* Extract filename */
		memcpy(filename1, p.payload, t.len - sizeof(int));
		sprintf(filename, "recv_");
		sprintf(filename + strlen(filename), "%s", filename1);
		printf("[RECEIVER] Filename: %s\n", filename);

		/* Send ACK for filename */	
		memset(t.payload, 0, sizeof(t.payload));
		memset(p.payload, 0, sizeof(p.payload));
	
		p.SEQ = -2;
		memcpy(p.payload, "Filename", strlen("Filename"));
	  	t.len = strlen(p.payload) + 1 + sizeof(int);
		memcpy(t.payload, &p, t.len);
		while(1) {
			res = send_message(&t);
			if (res < 0) {
				perror("[RECEIVER] ACK send error. Resending");
				continue;
			}
			break;
		}

		/* Wait for filesize */
		memset(t.payload, 0, sizeof(t.payload));
		if (recv_message(&t) < 0) {
			perror("[RECEIVER] Received msg error");
			return -1;
		}

		p = *((my_pkt*) t.payload);
		if (p.SEQ != -1) {
			perror("[RECEIVER] Received WRONG SEQ");
			return -1;
		}
	
		printf("[RECEIVER] Message SEQ: %d (Filesize)\n", p.SEQ);
		memcpy(&filesize, p.payload, sizeof(int));
		printf("[RECEIVER] Filesize: %d\n", filesize);

		/* Send ACK for filesize */
		memset(t.payload, 0, sizeof(t.payload));
		memset(p.payload, 0, sizeof(p.payload));
	
		p.SEQ = -1;
		memcpy(p.payload, "Filesize", strlen("Filesize"));
	  	t.len = strlen(p.payload) + 1 + sizeof(int);
		memcpy(t.payload, &p, t.len);
		while(1) {
			res = send_message(&t);
			if (res < 0) {
				perror("[RECEIVER] ACK send error. Resending");
				continue;
			}
			break;
		}

		read_so_far = 0;
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);

		/* Wait for file contents - (SEQ = 0 -> COUNT) messages */
		printf("[RECEIVER] Gonna wait for file chunks.\n");

		int seq = 0;
		while (read_so_far < filesize) {
			memset(t.payload, 0, sizeof(t.payload));
		
			if (recv_message(&t) < 0) {
				perror("[RECEIVER] Received msg error");
				continue;
			}
		
			p = *((my_pkt*) t.payload);
			if (p.SEQ != seq) {
				perror("[RECEIVER] Received WRONG SEQ");
				continue;
			}

			read_so_far += t.len - sizeof(int);
			write(fd, p.payload, t.len - sizeof(int));;
			printf("[RECEIVER] Got msg with SEQ: %d\n", p.SEQ);
		
			memset(t.payload, 0, sizeof(t.payload));
			memset(p.payload, 0, sizeof(p.payload));
	
			p.SEQ = ++seq;
			memcpy(p.payload, "ACK", strlen("ACK"));
		  	t.len = strlen(p.payload) + 1 + sizeof(int);
			memcpy(t.payload, &p, t.len);
			while(1) {
				res = send_message(&t);
				if (res < 0) {
					perror("[RECEIVER] ACK send error. Resending");
					continue;
				}
				break;
			}
		}

		close(fd);
	}

	printf("[RECEIVER] All done.\n");
	
	return 0;
}
