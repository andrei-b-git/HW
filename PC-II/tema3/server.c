/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX_CLIENTS	10
#define BUFLEN 256

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {

	int sockfd, newsockfd, portno, clilen;
	char buffer[BUFLEN];
	char* buff;
	struct sockaddr_in serv_addr, cli_addr;
	int n, m, i, j, k;

	fd_set read_fds;	//multimea de citire folosita in select()
	fd_set tmp_fds;	//multime folosita temporar
	int fdmax;		//valoare maxima file descriptor din multimea read_fds

	int clients[MAX_CLIENTS];					//vector de clienti conectati
	char client_name[MAX_CLIENTS][BUFLEN];  	//numele clientilor
	time_t client_time[MAX_CLIENTS]; 			//timpul
	int client_listen_port[MAX_CLIENTS];		//porturile pe care asculta clientii
	struct sockaddr_in client_addr[MAX_CLIENTS];//IP-ul clientilor

	char* aux = "";
	char s[512] = "";
	aux = (char *)malloc(512);
	double t;

	if(argc < 2) {
		fprintf(stderr,"Usage : %s port\n", argv[0]);
		exit(1);
	}

	//golim multimea de descriptori de citire (read_fds) si multimea tmp_fds
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("ERROR opening socket");

	portno = atoi(argv[1]);

	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;	// foloseste adresa IP a masinii
	serv_addr.sin_port = htons(portno);

	if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(struct sockaddr)) < 0)
		error("ERROR on binding");

	listen(sockfd, MAX_CLIENTS);

	//adaugam noul file descriptor (socketul pe care se asculta conexiuni) in multimea read_fds
	FD_SET(sockfd, &read_fds);
	FD_SET(0, &read_fds);
	fdmax = sockfd;

	// main loop
	while (1) {
		aux = "";
		tmp_fds = read_fds;
		if(select(fdmax + 1, &tmp_fds, NULL, NULL, NULL) == -1)
			error("ERROR in select");

		for(i = 0; i <= fdmax; i++) {
			if(FD_ISSET(i, &tmp_fds)) {
				if(i == 0) {
					//s-a introdus o comanda de la tastatura
					size_t nbytes = 100;
					buff = (char *) malloc (nbytes + 1);
					n = getline(&buff,&nbytes, stdin);
					if(strcmp(buff,"status\n") == 0) {
						for(k = 0; k < MAX_CLIENTS; k++) {
							if(clients[k] != 0) {
								printf("Clientul %s:\n",client_name[k]);
								printf("Adresa IP: %s\n", inet_ntoa(client_addr[k].sin_addr));
								printf("Portul de comunicare cu serverul: %d\n",ntohs(client_addr[k].sin_port));
								printf("Portul de ascultare a clientului: %d\n",client_listen_port[k]);
								printf("\n");
							}
						}
					}
					else
						if(strcmp(buff,"quit\n") == 0) {
							for(k = 0; k < MAX_CLIENTS; k++) {
								if(clients[k] > 0) {
									close(clients[k]);
								}
							}
							close(sockfd);
							printf("Quitting server\n");
							exit(0);
						}
						else
							printf ("Comanda invalida\n");
					fflush(stdin);
				}
				else
					if (i == sockfd) {
						// a venit ceva pe socketul de ascultare = o noua conexiune
						// actiunea serverului: accept()
						clilen = sizeof(cli_addr);
						if ((newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
							error("ERROR in accept");
						}
						else {
							//adaug noul socket intors de accept() la multimea descriptorilor de citire
							memset(buffer, 0, BUFLEN);
							n = recv(newsockfd, buffer, sizeof(buffer), 0);
							//daca exista deja un client cu acest nume, respinge conexiunea
							int client_exists = 0;
							for(k = 0; k < MAX_CLIENTS; k++) {
								if(clients[k] != 0 && strstr(buffer,client_name[k]) == buffer)
									client_exists = 1;
							}
							if(client_exists == 1) {
								aux = "Conexiune refuzata: exista deja un client cu acest nume";
								m = send(newsockfd,aux,strlen(aux),0);
								close(newsockfd);
							}
							else {
								//se accepta conexiunea si se salveaza informatiile corespunzatoare clientului
								for(k = 0; k < MAX_CLIENTS; k++)
									if(clients[k] == 0) {
										clients[k] = newsockfd;
										client_addr[k] = cli_addr;
										time(&client_time[k]);
										aux = (char *)malloc(512);
										int port = 0;
										sscanf(buffer,"%s %d",aux,&port);
										strcpy(client_name[k],aux);
										client_listen_port[k] = port;
										aux = "Conectat";
										m = send(newsockfd,aux,strlen(aux),0);
										break;
									}
								FD_SET(newsockfd, &read_fds);
								if (newsockfd > fdmax) {
									fdmax = newsockfd;
								}
								printf("Noua conexiune de la %s, port %d, socket_client %d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port), newsockfd);
								printf("Numele clientului : %s\nPortul pe care asculta  : %d\n", client_name[k],client_listen_port[k]);
							}
						}
					}
					else {
						// am primit date pe unul din socketii cu care vorbesc cu clientii
						//actiunea serverului: recv()
						memset(buffer, 0, BUFLEN);
						if((n = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
							aux = (char*) malloc(512);
							if(n == 0) {
								//conexiunea s-a inchis
								for(k = 0; k < MAX_CLIENTS; k++)
									if(clients[k] == i) {
										printf("clientul %s a inchis\n",client_name[k]);
										clients[k] = 0;
									}
							}
							else {
								error("ERROR in recv");
							}
							close(i);
							FD_CLR(i, &read_fds);
						}
						else { //recv intoarce >0
							if(strcmp(buffer,"listclients\n") == 0) {
								aux = (char *)malloc(512);
								strcpy(aux, "lista de clienti:\n");
								for(k = 0; k < MAX_CLIENTS; k++) {
									if(clients[k]) {
										strcat(aux, client_name[k]);
										strcat(aux, "\n");
									}
								}
								strcat(aux,"\0");
								m = send(i,aux,strlen(aux),0);
							}
							else
								if(strstr(buffer,"infoclient") == buffer) {
									aux = buffer + 11; aux[strlen(aux)-1] = '\0';
									for(k = 0; k < MAX_CLIENTS; k++) {
										if(clients[k] != 0 && strcmp(aux,client_name[k]) == 0) {
											time_t x;
											time(&x);
											t = difftime(x,client_time[k]);
											sprintf(aux, "Informatii despre %s:\nPort: %d\nTimp: %.2lf\n",client_name[k], client_listen_port[k],t);
											m = send(i,aux,strlen(aux),0);
											break;
										}
									}
									if(k == MAX_CLIENTS) {
										sprintf(aux, "Nu exista niciun client conectat cu acest nume!\n");
										m = send(i,aux,strlen(aux),0);
									}
								}
								else
									if(strstr(buffer,"message") == buffer) {
										aux = buffer + 8;
										aux[strlen(aux)-1] = '\0';
										for(k = 0; k < MAX_CLIENTS; k++) {
											if(clients[k] != 0 && strcmp(aux,client_name[k]) == 0) {
												sprintf(aux,"%s %d\n",inet_ntoa(client_addr[k].sin_addr), client_listen_port[k]);
												m = send(i,aux,strlen(aux),0);
												break;
											}
										}
										if(k == MAX_CLIENTS) {
											sprintf(aux, "Eroare: Nu exista niciun client conectat cu acest nume!\n");
											m = send(i,aux,strlen(aux),0);
										}
									}
									else
										if(strstr(buffer,"sendfile") == buffer) {
											char* w1 = (char*)malloc(512);
											char* w2 = (char*)malloc(512);
											char* w3 = (char*)malloc(512);
											sscanf(buffer,"%s %s %s\n",w1,w2,w3);
											int j = 0;
											while(clients[j] != i)
												j++;
											for(k = 0; k < MAX_CLIENTS; k++) {
												if(clients[k] != 0 && strcmp(w2,client_name[k]) == 0) {
													strcpy(s,"");
													sprintf(s,"sendfile %s %d %s\n",inet_ntoa(client_addr[j].sin_addr), client_listen_port[j],w3);
													m = send(clients[k],s,strlen(s),0);
													strcpy(s,"");
													sprintf(s,"Cerere trimisa\n");
													m = send(i,s,strlen(s),0);
													break;
												}
											}
											if(k == MAX_CLIENTS) {
												sprintf(s, "Eroare: Nu exista niciun client conectat cu acest nume!\n");
												m = send(i,s,strlen(s),0);
											}
										}
										else
											printf("Comanda incorecta\n");
						}
					}
			}
		}
	}

	close(sockfd);

	return 0;
}
