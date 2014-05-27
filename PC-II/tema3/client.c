/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/time.h>

#define MAX_CLIENTS	10
#define BUFLEN 256
#define MAX_FILES 100

void error(char *msg)
{
	perror(msg);
	exit(0);
}

int main(int argc, char *argv[]) {

	int sockfd, sockclient, socksend, newsockfd, n, i, j, k, clilen;
	struct sockaddr_in serv_addr, client_addr, send_addr, cli_addr;
	struct hostent *server;

	fd_set read_fds;	//multimea de citire folosita in select()
	fd_set tmp_fds;		//multime folosita temporar
	int fdmax;		//valoare maxima file descriptor din multimea read_fds

	int files_to_send[MAX_FILES];
	char file_name[MAX_FILES][BUFLEN];	//numele fisierelor de trimis
	struct sockaddr_in client_dest_addr[MAX_FILES];	//adresa clientului destinatie
	int files_to_receive[MAX_FILES];
	char file_name_to_receive[MAX_FILES][BUFLEN];
	char message[1024];
	char* aux;
	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 5000;

	char buffer[BUFLEN];
	if(argc < 4) {
		fprintf(stderr,"Usage %s nume_client client_port server_IP server_port\n", argv[0]);
		exit(0);
	}

	for(j = 0; j < MAX_FILES; j++)
		files_to_send[j] = 0;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		error("ERROR opening socket");

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(atoi(argv[4]));
	inet_aton(argv[3], &serv_addr.sin_addr);

	if(connect(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)) < 0)
		error("ERROR connecting");

	//golim multimea de descriptori de citire (read_fds) si multimea tmp_fds
	FD_ZERO(&read_fds);
	FD_ZERO(&tmp_fds);

	//setam un socket pe care asculta clientul
	sockclient = socket(AF_INET, SOCK_STREAM, 0);
	if(sockclient < 0)
		error("ERROR opening socket");

	int portno = atoi(argv[2]);

	memset((char *) &client_addr, 0, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = INADDR_ANY;	// foloseste adresa IP a masinii
	client_addr.sin_port = htons(portno);
	int b;
	if((b = bind(sockclient, (struct sockaddr *) &client_addr, sizeof(struct sockaddr))) < 0)
		error("ERROR on binding");

	socklen_t sock_t = sizeof(client_addr);
	int x = getsockname(sockclient, (struct sockaddr *) &client_addr, &sock_t);

	listen(sockclient, MAX_CLIENTS*1000);
	FD_SET(STDIN_FILENO, &read_fds);
	FD_SET(sockfd, &read_fds);
	FD_SET(sockclient, &read_fds);
	fdmax = sockclient;

	//trimite numele clientului si portul pe care asculta
	memset(buffer, 0, BUFLEN);
	sprintf(buffer,"%s %d",argv[1],ntohs(client_addr.sin_port));
	n = send(sockfd, buffer, strlen(buffer), 0);
	memset(buffer, 0 , BUFLEN);
	n = recv(sockfd, buffer, sizeof(buffer), 0);
	printf("%s\n",buffer);
	if(strcmp(buffer,"Conectat") != 0) {
		exit(1);
	}

	while(1) {
		tv.tv_sec = 0;
		tv.tv_usec = 0;
		//trimite un pachet daca mai exista fisiere de trimis
		j = 0;
		while(files_to_send[j] == 0) {
			j++;
			if(j == MAX_FILES) break;
		}
		if(j != MAX_FILES) {
			socksend = socket(AF_INET, SOCK_STREAM, 0);
			if (socksend < 0)
				error("ERROR opening socket");
			if (connect(socksend,(struct sockaddr*) &client_dest_addr[j],sizeof(client_dest_addr[j])) < 0)
				error("ERROR connecting");
			memset(buffer, 0 , BUFLEN);
			memset(message, 0, 1024);
			strcpy(buffer, "Sending ");
			strcat(buffer, file_name[j]);
			strcat(buffer,"\n");
			//trimite mesajul la clientul destinatar
			int r = 0;
			if( (r = read(files_to_send[j],&message,1024)) > 0) {
				sprintf(aux,"%d\n",r);
				strcat(buffer,aux);
				for(k = strlen(buffer); k < 256; k++)
					buffer[k] = 65;
				n = send(socksend,buffer,strlen(buffer),0);
				n = send(socksend,message,r,0);
			}
			else {
				close(files_to_send[j]);
				files_to_send[j] = 0;
				strcat(buffer,"0\n");
				printf("Am terminat de trimis fisierul %s\n",file_name[j]);
				n = send(socksend,buffer,strlen(buffer),0);
			}
			close(socksend);
		}

		tmp_fds = read_fds;
		if (select(sockclient + 1, &tmp_fds, NULL, NULL, &tv) == -1)
			error("ERROR in select");

		for(i = 0; i <= fdmax; i++) {
			if(FD_ISSET(i, &tmp_fds)) {
				if(i == STDIN_FILENO) {
					//citesc de la tastatura
					memset(buffer, 0 , BUFLEN);
					fgets(buffer, BUFLEN-1, stdin);
					aux = (char *)malloc(512);
					strcpy(aux, buffer);
					char* word = (char *)malloc(512);
					word = strtok(aux," ");
					if(strcmp(word,"infoclient") == 0 ||
						strcmp(word,"listclients\n") == 0) {
						//trimit mesaj la server
						n = send(sockfd,buffer,strlen(buffer), 0);
						if (n < 0)
							error("ERROR writing to socket");
					}
					else
						if(strcmp(word,"message") == 0) {
							word = strtok(NULL," ");
							sprintf(buffer,"message %s\n",word);
							n = send(sockfd,buffer,strlen(buffer), 0);
							if (n < 0)
								error("ERROR writing to socket");
							strcpy(buffer,aux + strlen(word) +9);
							buffer[strlen(buffer)-1] = '\0';
							char* mesaj = (char *)malloc(512);
							strcpy(mesaj,"Mesaj de la ");
							strcat(mesaj,argv[1]);
							strcat(mesaj,":\n");
							strcat(mesaj,buffer);
							int port = 0;
							//raspunsul de la server cu IP-ul si portul clientului destinatie
							n = recv(sockfd, buffer, sizeof(buffer), 0);
							if(strstr(buffer,"Eroare") == buffer)
								printf("%s",buffer);
							else {
								sscanf(buffer,"%s %d",aux,&port);
								socksend = socket(AF_INET, SOCK_STREAM, 0);
								if (socksend < 0)
									error("ERROR opening socket");

								send_addr.sin_family = AF_INET;
								send_addr.sin_port = htons(port);
								inet_aton(aux, &send_addr.sin_addr);

								if (connect(socksend,(struct sockaddr*) &send_addr,sizeof(send_addr)) < 0)
									error("ERROR connecting");
								//trimite mesajul la clientul destinatar
								n = send(socksend,mesaj,strlen(mesaj), 0);
								close(socksend);
							}
						}
						else
							if(strcmp(word,"sendfile") == 0) {
								word = strtok(NULL," ");
								word = strtok(NULL," ");
								printf("Fisierul de trimis: %s\n",word);
								n = send(sockfd,buffer,strlen(buffer), 0);
								if (n < 0)
									error("ERROR writing to socket");
								n = recv(sockfd, buffer, sizeof(buffer), 0);
								printf("%s",buffer);
							}
							else
								if(strcmp(word,"quit\n") == 0) {
									printf("Quitting client\n");
									close(sockfd);
									close(sockclient);
									exit(0);
								}
								else {
									printf("Comanda incorecta!\n");
								}
				}
				else
					if(i == sockfd) {
						//a venit ceva de la server
						memset(buffer, 0, BUFLEN);
						if ((n = recv(sockfd, buffer, sizeof(buffer), 0)) <= 0) {
							if (n == 0) {
								//conexiunea s-a inchis
								printf("server hung up\n");
							}
							else {
								error("ERROR in recv");
							}
							close(sockfd);
							close(sockclient);
							exit(0);
						}
						else { //recv intoarce >0
							if(strstr(buffer,"getfile") == buffer) {
								j = 0;
								while(files_to_send[j] != 0)
									j++;
								char* w1 = (char*)malloc(512);
								int port = 0;
								sscanf(buffer,"%s %s %d %s\n",w1,aux,&port,file_name[j]);
								files_to_send[j] = open(file_name[j], O_RDONLY);

								client_dest_addr[j].sin_family = AF_INET;
								client_dest_addr[j].sin_port = htons(port);
								inet_aton(aux, &client_dest_addr[j].sin_addr);
							}
							else {
								printf ("%s\n", buffer);
							}
						}
				}
				else
					if(i == sockclient) {
						clilen = sizeof(cli_addr);
					if ((newsockfd = accept(sockclient, (struct sockaddr *)&cli_addr, &clilen)) == -1) {
						error("ERROR in accept");
					}
					memset(buffer, 0, BUFLEN);
					n = recv(newsockfd, buffer, sizeof(buffer), 0);
					if(strstr(buffer,"Sending") == buffer) {
						char* w1 = (char*)malloc(512);
						char* w2 = (char*)malloc(512);
						char* nume = (char*)malloc(512);
						int nr = 0;
						sscanf(buffer,"%s %s %d\n",w1,w2,&nr);
						for(j = 0; j < MAX_FILES; j++) {
							if(files_to_receive[j] > 0 && strcmp(file_name_to_receive[j],w2) == 0) {
								break;
							}
						}
						if(j == MAX_FILES) {
							j = 0;
							while(files_to_receive[j] != 0)
								j++;
							strcpy(file_name_to_receive[j],w2);
							strcpy(nume,w2);
							strcat(nume,"_primit");
							files_to_receive[j] = open(nume,O_WRONLY|O_CREAT,0644);
						}
						if(nr == 0) {
							printf("Am terminat de primit fisierul %s\n",w2);
							files_to_receive[j] = 0;
						}
						else {
							memset(message, 0, 1024);
							n = recv(newsockfd, message, sizeof(message), 0);
							write(files_to_receive[j], message, nr);
						}
					}
					else {
						printf("%s\n",buffer);
					}
					close(newsockfd);
				}
			}
		}
		usleep(4000);
	}
	close(sockfd);
	close(sockclient);
	return 0;
}
