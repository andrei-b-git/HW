#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "linked_list.h"
#include "graph.h"

#define MAX 100000 //dimensiunea bufferelor de tip char * care 
				   //ma ajuta la citirea din fisiere

int main(int argc, char **argv)
{
	FILE *f1=NULL, *f2=NULL;
	f1=fopen(argv[1],"r"); //deschiderea fisierului .in
	f2=fopen(argv[2],"r"); //deschiderea fisierului .txt
	int N,M,i;
	char n[MAX],m[MAX],s[MAX],z[MAX],*p; //buffere pt citire
	fgets(n,MAX,f1);
	fgets(m,MAX,f2);
	N=atoi(n); //numarul de taskuri de procesat
	M=atoi(m); //numarul de procesoare din sistem
	Graph<int> g(N);
	for(i=1;i<=N;i++) //citim N linii din fisiere
	{
		fgets(s,MAX,f1); //citirea sirului de dependente
		fgets(z,MAX,f2); //citirea timpului fiecarui task
		p=strtok(s," "); //parsarea sirului de dependente
		while(p!=NULL)
		{
			g.addEdge(i,atoi(p)); //retinerea grafului sub forma de 
								  //liste de adiacenta
			p=strtok(NULL," ");
		}
		g.setNodeInfo(i,atoi(z)); //retinerea in graph a timpului
	}
	g.planificare(M); //apelarea metodei de planificare a taskurilor
	fclose(f1); //inchiderea fisierelor
	fclose(f2);
    return 0;
}
