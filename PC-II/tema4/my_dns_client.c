/* nume: Bojin
 * prenume: Andrei Florian
 * grupa/seria: 323 CA
 */

#include<stdio.h>
#include "dns_message.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include<stdlib.h>
#include <arpa/inet.h>
#include<string.h>

unsigned char* ReadName(unsigned char* reader,unsigned char* buffer,int* count) {
	unsigned char *name;
	unsigned int p=0, jumped=0, offset;
	int i, j;
	*count = 1;
	name = (unsigned char*)malloc(256);
	name[0]='\0';
	//citeste numele in format 3www6google3com
	while(*reader!=0) {
		if(*reader>=192) {
			offset = (*reader)*256 + *(reader+1) - 49152; //49152 = 11000000 00000000 in binar
			reader = buffer + offset - 1;
			jumped = 1; //am sarit la alta locatie deci contorul nu va creste
		}
		else {
			name[p++]=*reader;
		}
		reader=reader+1;
		if(jumped==0) *count = *count + 1; //daca nu am sarit la alta locatie contorul poate creste
	}
	name[p]='\0'; //string-ul complet
	if(jumped==1) {
		*count = *count + 1; //numarul de pasi cu care am avansat practic in packet
	}
	//convertim 3www6google3com0 la www.google.com
	for(i=0;i<(int)strlen((const char*)name);i++)
	{
		p=name[i];
		for(j=0;j<(int)p;j++) {
			name[i]=name[i+1];
			i=i+1;
		}
		name[i]='.';
	}
	name[i-1]='\0';
	return name;
}

int main(int argc, char ** argv) {

	unsigned char *domainName = (unsigned char*)strdup(argv[1]);
	int type;
	if(strcmp("A", argv[2]) == 0)
		type = A;
	else
		if(strcmp("NS", argv[2]) == 0)
			type = NS;
		else
			if(strcmp("CNAME", argv[2]) == 0)
				type = CNAME;
			else
				if(strcmp("MX", argv[2]) == 0)
					type = MX;
				else
					if(strcmp("SOA", argv[2]) == 0)
						type = SOA;
					else
						if(strcmp("TXT", argv[2]) == 0)
							type = TXT;

	FILE *f = fopen("dns_servers.conf", "r");
	if(!f)
		printf("ERROR in opening file!");

	char line[128];
	while (fgets(line, sizeof(line), f) != NULL)
		if(strcmp(line, "\n") != 0 && line[0] != '#') {
			line[strlen(line)-1] = 0;
			printf("%s\n", line);
		}
	fclose(f);

	dns_header_t *dns_header = NULL;
	unsigned char buf[65536];
	dns_header = (struct dns_header_t*)&buf;
	dns_header->id = (unsigned short) htons(13);
	dns_header->rd = 1;
	dns_header->tc = 0;
	dns_header->aa = 0;
	dns_header->opcode = 0;
	dns_header->qr = 0;

	dns_header->rcode = 0;
	dns_header->z = 0;
	dns_header->ra = 0;

	dns_header->qdcount = htons(1);
	dns_header->ancount = 0;
	dns_header->nscount = 0;
	dns_header->arcount = 0;

	unsigned char *qname = (unsigned char*)&buf[sizeof(dns_header_t)];
	dns_question_t *dns_question = NULL;

	int lock=0;
	int i;
	strcat((char*)domainName,".");
	for(i=0 ; i<(int)strlen((char*)domainName) ; i++) {
		if(domainName[i]=='.') {
			*qname++=i-lock;
			for(;lock<i;lock++) {
				*qname++=domainName[lock];
			}
			lock++;
		}
	}
	*qname++='\0';

	printf("%s\n", qname);
	dns_question = (struct dns_question_t*)&buf[sizeof(dns_header_t) + (strlen((const char*)qname) + 1)];
	dns_question->qtype = htons(type);
	dns_question->qclass = htons(1);

	struct sockaddr_in remote_saddr, a;
	remote_saddr.sin_family = AF_INET;
	remote_saddr.sin_port  = htons(53);
	inet_aton("213.154.124.221", (struct in_addr *)&remote_saddr.sin_addr.s_addr);
	int fd = socket(AF_INET, SOCK_DGRAM, 0);
	struct timeval tv;
	tv.tv_sec = 3;
	tv.tv_usec = 0;
	setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(tv));
	int len = sizeof(dns_header_t) + (strlen((const char*)qname)+1) + sizeof(dns_question_t);
	if(sendto(fd, (char*)buf, len, 0, (struct sockaddr*)&remote_saddr, sizeof(remote_saddr)) < -1)
		printf("error la send\n");

	i = sizeof(remote_saddr);
	int load;
	if((load = recvfrom (fd, (char*)buf, 65536, 0, (struct sockaddr*)&remote_saddr, &i)) < 0)
		printf("eroare la recv\n");

	dns_header_t *answer_header;
	answer_header = (struct dns_header_t*)buf;
	unsigned char *reader;
	reader = &buf[sizeof(dns_header_t) + (strlen((const char*)qname)+1) + sizeof(dns_question_t)];

	printf("\nThe response contains : ");
	printf("\n %d Questions.",ntohs(answer_header->qdcount));
	printf("\n %d Answers.",ntohs(answer_header->ancount));
	printf("\n %d Authoritative Servers.",ntohs(answer_header->nscount));
	printf("\n %d Additional records.\n\n",ntohs(answer_header->arcount));

	//answer
	int stop = 0;
	int j;
	RES_RECORD answers[20];
	for(i=0;i<ntohs(answer_header->ancount);i++) {
		answers[i].name=ReadName(reader,buf,&stop);
		reader = reader + stop;

		answers[i].resource = (dns_rr_t *)(reader);
		reader = reader + sizeof(dns_rr_t);

		if(ntohs(answers[i].resource->type) == 1) {
			answers[i].rdata = (unsigned char*)malloc(ntohs(answers[i].resource->rdlength));
			for(j=0 ; j<ntohs(answers[i].resource->rdlength) ; j++)
				answers[i].rdata[j]=reader[j];
			answers[i].rdata[ntohs(answers[i].resource->rdlength)] = '\0';
			reader = reader + ntohs(answers[i].resource->rdlength);
		}
		else if(ntohs(answers[i].resource->type) == MX) {
			answers[i].rdata = (unsigned char*)malloc(ntohs(answers[i].resource->rdlength));
			answers[i].rdata[0] = reader[0];
			answers[i].rdata[1] = reader[1];
			reader = reader + 2;
			strcpy(answers[i].rdata + 2, ReadName(reader,buf,&stop));
			reader = reader + stop;
		}
		else {
			answers[i].rdata = ReadName(reader,buf,&stop);
			reader = reader + stop;
		}
	}

	//afisare answer
	for(i=0;i<ntohs(answer_header->ancount);i++) {
		printf("%s %d %d ",answers[i].name, ntohs(answers[i].resource->class), ntohs(answers[i].resource->type));

		if(ntohs(answers[i].resource->type)==1) {
			long *p;
			p=(long*)answers[i].rdata;
			a.sin_addr.s_addr=(*p);
			printf("%s",inet_ntoa(a.sin_addr));
		}
		if(ntohs(answers[i].resource->type)==5) {
			printf("has alias name : %s",answers[i].rdata);
		}
		printf("\n");
	}
	printf("\n");

	//authorities
	RES_RECORD auth[20];
	for(i=0 ; i < ntohs(answer_header->nscount) ; i++) {
		auth[i].name = ReadName(reader, buf, &stop);
		reader+=stop;
		auth[i].resource = (dns_rr_t *)(reader);
		reader += sizeof(dns_rr_t);
		auth[i].rdata = ReadName(reader,buf,&stop);
		reader += stop;
	}

	//afisare auhorities
	for(i=0 ; i<ntohs(answer_header->nscount) ; i++) {
		printf("%s %d %d ",auth[i].name, ntohs(auth[i].resource->class), ntohs(auth[i].resource->type));
		if(ntohs(auth[i].resource->type)==2)
			printf("%s",auth[i].rdata);
		printf("\n");
	}
	printf("\n");

	//additional
	RES_RECORD addit[20];
	for(i = 0 ; i<ntohs(answer_header->arcount) ; i++) {
		addit[i].name=ReadName(reader,buf,&stop);
		reader+=stop;
		addit[i].resource=(dns_rr_t *)(reader);
		reader+=sizeof(dns_rr_t);
		if(ntohs(addit[i].resource->type)==1) {
			addit[i].rdata = (unsigned char*)malloc(ntohs(addit[i].resource->rdlength));
			for(j=0;j<ntohs(addit[i].resource->rdlength);j++)
				addit[i].rdata[j]=reader[j];
			addit[i].rdata[ntohs(addit[i].resource->rdlength)]='\0';
			reader+=ntohs(addit[i].resource->rdlength);
		}
		else if(ntohs(answers[i].resource->type) == MX) {
			answers[i].rdata = (unsigned char*)malloc(ntohs(answers[i].resource->rdlength));
			answers[i].rdata[0] = reader[0];
			answers[i].rdata[1] = reader[1];
			reader = reader + 2;
			strcpy(answers[i].rdata + 2, ReadName(reader,buf,&stop));
			reader = reader + stop;
		}
		else {
			addit[i].rdata=ReadName(reader,buf,&stop);
			reader+=stop;
		}
	}

	//afisare additional
	for(i=0 ; i<ntohs(answer_header->arcount) ; i++) {
		printf("%s %d %d ",addit[i].name, ntohs(addit[i].resource->class), ntohs(addit[i].resource->type));
		if(ntohs(addit[i].resource->type)==1) {
			long *p;
			p=(long*)addit[i].rdata;
			a.sin_addr.s_addr=(*p);
			printf("%s",inet_ntoa(a.sin_addr));
		}
		printf("\n");
	}
	printf("\n");

	return 0;
}
