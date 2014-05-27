#include "helpers.h"
#include "io_lib.h"
#include "simulation_messages.h"

//NU AVETI VOIE SA MODIFICATI acest fisier!
//;▌───▐██▄───────────────▄▓░░▓▓────▐
//;▌───▐█░██▓────────────▓▓░░░▓▌────▐
//;▌───▐█▌░▓██──────────█▓░░░░▓─────▐
//;▌────▓█▌░░▓█▄███████▄███▓░▓█─────▐
//;▌────▓██▌░▓██░░░░░░░░░░▓█░▓▌─────▐
//;▌─────▓█████░░░░░░░░░░░░▓██──────▐
//;▌─────▓██▓░░░░░░░░░░░░░░░▓█──────▐
//;▌─────▐█▓░░░░░░█▓░░▓█░░░░▓█▌─────▐
//;▌─────▓█▌░▓█▓▓██▓░█▓▓▓▓▓░▓█▌─────▐
//;▌─────▓▓░▓██████▓░▓███▓▓▌░█▓─────▐
//;▌────▐▓▓░█▄▐▓▌█▓░░▓█▐▓▌▄▓░██─────▐
//;▌────▓█▓░▓█▄▄▄█▓░░▓█▄▄▄█▓░██▌────▐
//;▌────▓█▌░▓█████▓░░░▓███▓▀░▓█▓────▐
//;▌───▐▓█░░░▀▓██▀░░░░░─▀▓▀░░▓█▓────▐
//;▌───▓██░░░░░░░░▀▄▄▄▄▀░░░░░░▓▓────▐
//;▌───▓█▌░░░░░░░░░░▐▌░░░░░░░░▓▓▌───▐
//;▌───▓█░░░░░░░░░▄▀▀▀▀▄░░░░░░░█▓───▐
//;▌──▐█▌░░░░░░░░▀░░░░░░▀░░░░░░█▓▌──▐
//;▌──▓█░░░░░░░░░░░░░░░░░░░░░░░██▓──▐
//;▌──▓█░░░░░░░░░░░░░░░░░░░░░░░▓█▓──▐
//;▌──██░░░░░░░░░░░░░░░░░░░░░░░░█▓──▐
//;▌──█▌░░░░░░░░░░░░░░░░░░░░░░░░▐▓▌─▐
//;▌─▐▓░░░░░░░░░░░░░░░░░░░░░░░░░░█▓─▐
//;▌─█▓░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓─▐
//;▌─█▓░░░░░░░░░░░░░░░░░░░░░░░░░░▓▓▌▐
//;▌▐█▓░░░░░░░░░░░NO░░░░░░░░░░░░░▓▓▌▐



int simulare(int pipes_fds[KIDS][4], event_t *events, int no_events)
{
	/* pipes_fds[k][0] - used for receiving messages from node */
	/* pipes_fds[k][3] - used for sending messages to node 	   */
	
	
	int i;
	msg message;

	/* select() fd set */
	fd_set read_fds;

	/* auxiliary fd set */
	fd_set tmp_fds;
	
	/* highest file descriptor ID */
	int fdmax= 0;
	int time = -1;

	/* last added node */
	int last_added_node = 0; // it has the highest node/router ID

	/* number of router instances that finished processing at time t-1 */
	int no_closed_nodes = 0;

	/* number of received link state messages */
	int no_received_LS_messages = 0;

	/* file descriptor status after read/receive */
	int fd_status;

	/* auxiliary variable for nodes/router instances iteration */
	int last_node;

	/* fd sets initialization */
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);
	
	/* node 0 is the router that starts the chain of events      */
	/* so it will be the first one to be added for communication */
	FD_SET(pipes_fds[0][0], &read_fds);
	fdmax = pipes_fds[0][0];

	/* file used to log events */
	FILE *log_file;

	/* try opening the file */
	log_file = fopen("log_rutare","w");
	DIE (log_file == NULL, "fopen fisier iesire" );

	// main loop
	/* it ends when there are no more nodes alive OR */
	/* if there are no more events to trigger        */
	while((no_closed_nodes != last_added_node+1) || (time < no_events)) { 
	
		tmp_fds = read_fds; 
		
		DIE (select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) < 0, "select"); 
		
		for(i = 0; i <= fdmax; i++) {
			if (FD_ISSET(i, &tmp_fds)) {
				fd_status = read (i, &message, sizeof(msg));
			
				/* check status of the pipe used to receive messages from node i */
				DIE (fd_status < 0, "citire mesaj simulator" );
			
				if (fd_status == CHILD_DONE_AND_CLOSED_CONNECTION)
					continue;
				
				switch (message.msg_type) {
			
				// process link state messages
				case M1: case M2: case M3: case M4: {
					routeMessage(&message, pipes_fds[message.next_hop][3], time, log_file, last_added_node);
					no_received_LS_messages++;
					break;
				}
				
				// finalize message processing at time stamp t
				case M5: { /* also triggers M7, M8, M9 and M6 messages */
					printf ("Timp %d, Simulator , msg tip %d de la %d\n", time, message.msg_type, message.sender);

					/* count how many router instances finished processing at time t-1 */
					no_closed_nodes++;

					message.sender = -1;
					
					last_node = last_added_node;
					
					/* if every node processed their messages at the current time */
					if (no_closed_nodes == last_added_node+1) {
						/* announce new events (send M7) */
						announceNewEvent(events, pipes_fds, &read_fds, &fdmax, time, no_events, &last_added_node);

						/* ask for routing table(send M8) */
						askForRoutingTables(pipes_fds, last_node);

						/* if there are no messages received & there are no other events*/
						if ((no_received_LS_messages == 0) && (time >= 2*no_events)) {
							/* send M9 */
							endSimulation(pipes_fds, log_file, last_node);
						} else {
							/* tell routers to further process their received messages */
							/* send M6 */
							time++;
							no_closed_nodes = 0;
							no_received_LS_messages = 0;
							announceFurtherMessageProcessing(pipes_fds, last_added_node);
						}


					}
					break;
				}
				
				/* log routing table */
				case M10: {
					logRoutingTable(&message, log_file);
					break;
				}
				
				default: 
					printf ("\n EROARE: Timp %d, Simulator, msg tip %d - NU PROCESEZ ACEST TIP DE MESAJ\n", time, message.msg_type);
					exit (-1);	
			
				}
			}
		}
	}
	fclose(log_file);
	return 0;
}


int main (int argc, char ** argv) {

	/* router processes */
	pid_t routers[KIDS];

	/* pipes used to communicate between the simulator and router instances */
	int pipes_fds[KIDS][4];

	/* event queue*/
	event_t * event_queue;
	int no_events;

	/* flag for process instances */ // (to make the simulation code execute only once)
	int child = FALSE;

	int k;
	
	event_queue = readEventsFromFile(argv[1], &no_events);
	
	child = createRouterInstances(routers, pipes_fds);

	if (!child) {
		/* execute simulation */
		DIE(simulare (pipes_fds, event_queue, no_events) < 0, "eroare in simulare");

		/* close router instances */
		for(k=0; k<KIDS;k++) {
			// wait for this child to finish
			DIE(waitpid (routers[k], NULL, 0) < 0, "waitpid");
			close (pipes_fds[k][0]);
			close (pipes_fds[k][3]);
		}
	}  //if

	return 0;
} //main

