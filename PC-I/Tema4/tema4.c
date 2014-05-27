/*
# nume: Bojin	
# prenume: Andrei Florian
# grupa 313 CA
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
# define MAX 1000000	//constanta folosita ca dimen
	
int *ind;

void initialize(unsigned char *arena)	//functia apelata la introducerea comenzii INITIALIZE <N>
{
	*(int *)arena=0;	//initializarea indicelui de start cu 0
}

void finalize(unsigned char *arena)	//functia apelata la introducerea comenzii FINALIZE
{
	free(arena);	//eliberarea memoriei alocate arenei
}

void dump(unsigned char *arena,int n)	//functia apelata la introducerea comenzii DUMP
{
	int i,j,k=0,m;
	long int addr=0;
	for(i=0;i<n/16;i++)
	{
		printf("%08lX\t",addr);		//afisarea indexului din stanga cu care incepe fiecare linie de octeti
	    	addr+=16;
		for(j=0,m=0;j<16;j++,m++)	
		{
			printf("%02X ",*arena++);	//afisarea octetilor
			if(m==7)
				printf(" ");
		}
		k+=16;
		printf("\n");
	}
	printf("%08lX\t",addr);		//afisarea indexului si a octetilor ramasi de pe ultima linie 
	for(i=k,m=0;i<n;i++,m++)
	{
		printf("%02X ",*arena++);
			if(m==7)
				printf(" ");
	}
	printf("\n");
	if(n%16!=0)	
		printf("%08lX\n",addr+m);	//afisarea si a indexului urmator la final
}

void alocare(unsigned char *arena,int n,int size,int *ind)	//functia apelata la introducerea comenzii ALLOC <SIZE>
{
	int bu,o=0,u=4;
	if(*(int *)arena==0)	//daca arena e goala
		if(size<=n-16)		//incercarea alocarii in ea
			{
				*(int *)arena=4;
				*(int *)(arena+4)=0;
				*(int *)(arena+8)=0;		//prelucrarea sectiunii de gestiune
				*(int *)(arena+12)=12+size;
				*ind=16;
				printf("16\n");
			}
		else
		{
			ind=0;		//retinerea indexului pe care treb sa-l afisez
			printf("0\n");
		}
	else
	{
		if(*(int *)arena-4>=size+12)	//incercarea alocarii in arena care contine macar un bloc, intre indicele de start si primul bloc
		{
			*(int *)(arena+4)=*(int *)arena;
			*(int *)(arena+8)=0;
			*(int *)(arena+12)=12+size;	//prelucrarea sectiunii de gestiune
			*(int *)(arena+*(int *)arena+4)=4;
			*(int *)arena=4;
			*ind=16;	
			printf("16\n");
		}
		else
		{
			if(*(int *)arena-4!=0)
				u=*(int *)arena;	//retin ultimul bloc din arena
			for(bu=*(int *)(arena+*(int *)arena);bu!=0;bu=*(int *)(arena+bu))
			{
				if(bu-(*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8))>=size+12)	//bu este indicele blocului urmator; realizez parcurgerea arenei in functie de bu
				{
					*ind=*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12;
					printf("%d\n",*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12);
					*(int *)(arena+*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8))=bu;		//prelucrarea sectiunii de gestiune
					*(int *)(arena+*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+4)=*(int *)(arena+bu+4);
					*(int *)(arena+*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+8)=size+12;
					*(int *)(arena+*(int *)(arena+bu+4))=*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8);
					*(int *)(arena+bu+4)=*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8);
					o=1;	//retin ca am alocat
					break;	
				}
				if(*(int *)(arena+bu)==0)
					u=bu;	//retin ultimul bloc din arena
			}
			if(o==0)	//daca nu am alocat pana acum
			{
				if(n-(u+*(int *)(arena+u+8))>=size+12)	//incerc alocarea in ultimul spatiu liber, dupa ultimul bloc, daca se poate
				{
					*ind=u+*(int *)(arena+u+8)+12;
					printf("%d\n",u+*(int *)(arena+u+8)+12);
					*(int *)(arena+u+*(int *)(arena+u+8))=0;
					*(int *)(arena+u+*(int *)(arena+u+8)+4)=u;	//prelucrarea sectiunii de gestiune
					*(int *)(arena+u+*(int *)(arena+u+8)+8)=size+12;
					*(int *)(arena+u)=u+*(int *)(arena+u+8);
				}
				else
				{
					*ind=0;
					printf("0\n");	//afisarea lui 0 in caz in care nu s-a putut aloca in arena
				}
			}
		}
	}
}

void eliberare(unsigned char *arena,int n,int index)	////functia apelata la introducerea comenzii FREE <INDEX>
{
	if(*(int *)(arena+index-12)!=0)
		*(int *)(arena+*(int *)(arena+index-12)+4)=*(int *)(arena+index-8);	//prelucrarea sectiunii de gestiune a blocurilor alaturate celui caruia vreau sa-i dau FREE
	*(int *)(arena+*(int *)(arena+index-8))=*(int *)(arena+index-12);
}

void umplere(unsigned char *arena,int n,int value,int size,int index)	//functia apelata la introducerea comenzii FILL <INDEX> <SIZE> <VALUE>
{
	int i;
	for(i=index;i<index+size;i++)	//copierea valorii value pe size octeti
		*(arena+i)=value;
}

void show(unsigned char *arena,int n,char *info)	//functia apelata la introducerea comenzii SHOW <INFO>
{
	int blib=0, lib=0, bu, ocu=0, bocu=0, u=4, a[MAX][2],l=0, i;	
//in a[MAX][2] retin octetii necesari lui SHOW ALLOCATIONS; in a[i][0] retin numarul de octeti iar in a[i][1] retin daca sunt ocupati(0) sau free(1)
	a[l][0]=4; a[l][1]=0; l++;
	if(*(int *)arena==0)
	{
		blib=1; lib=n-4;
		a[l][0]=n-4; a[l][1]=1; l++;
	}
	else
	{
		lib+=*(int *)arena-4;
		blib+=(lib!=0);
		if(lib!=0)
		{
			a[l][0]=*(int *)arena-4; a[l][1]=1; l++;
			u=*(int *)arena;
		}
		for(bu=*(int *)(arena+*(int *)arena);bu!=0;bu=*(int *)(arena+bu))
		{
			lib+=bu-(*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8));
			blib+=(bu-(*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8))!=0);
			ocu+=*(int *)(arena+*(int *)(arena+bu+4)+8)-12; bocu++;		//calculul numarului liber de octeti si al celor ocupati si al numarului de blocuri respective
			a[l][0]=*(int *)(arena+*(int *)(arena+bu+4)+8); a[l][1]=0; l++;
			if(bu-(*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8))!=0)
			{	
				a[l][0]=bu-(*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)); 
				a[l][1]=1; l++;
			}
			if(*(int *)(arena+bu)==0)
				u=bu;
		}
		lib+=n-(u+*(int *)(arena+u+8)); blib+=(n-(u+*(int *)(arena+u+8))!=0);
		ocu+=*(int *)(arena+u+8)-12; bocu++;
		a[l][0]=*(int *)(arena+u+8); a[l][1]=0; l++;
		if(n-(u+*(int *)(arena+u+8))!=0)
		{
			a[l][0]=n-(u+*(int *)(arena+u+8)); a[l][1]=1; l++;
		}
	}
	if(strcmp(info,"FREE\n")==0)
		printf("%d blocks (%d bytes) free\n",blib,lib);
	if(strcmp(info,"USAGE\n")==0)
	{
		printf("%d blocks (%d bytes) used\n",bocu,ocu);
		printf("%d%% efficiency\n",(int)((ocu*100)/(ocu+12*bocu+4)));
		if(bocu==0)
			printf("%d%% fragmentation\n",0);
		else
			printf("%d%% fragmentation\n",(int)(((blib-1)*100)/bocu));
	}
	if(strcmp(info,"ALLOCATIONS\n")==0)
	{	
		for(i=0;i<l;i++)
		{
			if(a[i][1]==0)
				printf("OCCUPIED ");
			else
				printf("FREE ");
			printf("%d bytes\n",a[i][0]);
		}
	}			
}

void allocaligne(unsigned char *arena,int n,int align,int size) 	//functia apelata la introducerea comenzii ALLOCALIGNED <SIZE> <ALIGN>
{	//in general allocaligne face acelasi lucru ca alloc deci sunt aceleasi comentarii ca la alloc
	int bu,o=0,u=4;
	if(*(int *)arena==0)
		if(size<=n-(16+(align-16)*(align-16>=0)))	
			{
				*(int *)arena=4+(align-16)*(align-16>=0);
				*(int *)(arena+(align-16)*(align-16>=0)+4)=0;
				*(int *)(arena+(align-16)*(align-16>=0)+8)=0;	//prelucrarea sectiunii de gestiune
				*(int *)(arena+(align-16)*(align-16>=0)+12)=12+size;
				printf("%d\n",16+(align-16)*(align-16>=0));
			}
		else
			printf("0\n");
	else
	{
		if(*(int *)arena-4>=size+12+(align-16)*(align-16>=0))
		{
			*(int *)(arena+(align-16)*(align-16>=0)+4)=*(int *)arena;
			*(int *)(arena+(align-16)*(align-16>=0)+8)=0;
			*(int *)(arena+(align-16)*(align-16>=0)+12)=12+size;	//prelucrarea sectiunii de gestiune
			*(int *)(arena+*(int *)arena+4)=4+(align-16)*(align-16>=0);
			*(int *)arena=4+(align-16)*(align-16>=0);	
			printf("%d\n",16+(align-16)*(align-16>=0));
		}
		else
		{
			if(*(int *)arena-4!=0)
				u=*(int *)arena;
			for(bu=*(int *)(arena+*(int *)arena);bu!=0;bu=*(int *)(arena+bu))
			{
				if(bu>=size+((int)((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)/align)+
				   ((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)%align!=0))*align)
				{
					printf("%d\n",((int)((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)/align)+
					 ((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)%align!=0))*align);
					*(int *)(arena+((int)((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)/align)+
					 ((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)%align!=0))*align-12)=bu;
					*(int *)(arena+((int)((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)/align)+
					 ((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)%align!=0))*align-8)=*(int *)(arena+bu+4);	//prelucrarea sectiunii de gestiune
					*(int *)(arena+((int)((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)/align)+
					 ((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)%align!=0))*align-4)=size+12;
					*(int *)(arena+*(int *)(arena+bu+4))=((int)((*(int *)(arena+bu+4)+*(int *)(arena+
					 *(int *)(arena+bu+4)+8)+12)/align)+((*(int *)(arena+bu+4)+*(int *)(arena+
					  *(int *)(arena+bu+4)+8)+12)%align!=0))*align-12;
					*(int *)(arena+bu+4)=((int)((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)/align)+
					 ((*(int *)(arena+bu+4)+*(int *)(arena+*(int *)(arena+bu+4)+8)+12)%align!=0))*align-12;
					o=1;
					break;	
				}
				if(*(int *)(arena+bu)==0)
					u=bu;
			}
			if(o==0)
			{
				if(n>=size+((int)((u+*(int *)(arena+u+8)+12)/align)+((u+*(int *)(arena+u+8)+12)%align!=0))*align)
				{
					printf("%d\n",((int)((u+*(int *)(arena+u+8)+12)/align)+((u+*(int *)(arena+u+8)+12)%align!=0))*align);
					*(int *)(arena+((int)((u+*(int *)(arena+u+8)+12)/align)+((u+*(int *)(arena+u+8)+12)%align!=0))*align-12)=0;
					*(int *)(arena+((int)((u+*(int *)(arena+u+8)+12)/align)+((u+*(int *)(arena+u+8)+12)%align!=0))*align-8)=u;	//prelucrarea sectiunii de gestiune
					*(int *)(arena+((int)((u+*(int *)(arena+u+8)+12)/align)+((u+*(int *)(arena+u+8)+12)%align!=0))*align-4)=size+12;
					*(int *)(arena+u)=((int)((u+*(int *)(arena+u+8)+12)/align)+((u+*(int *)(arena+u+8)+12)%align!=0))*align-12;
				}
				else
					printf("0\n");
			}
		}
	}
}

void realocare(unsigned char *arena,int n,int size,int index,int *ind)	//functia apelata la introducerea comenzii REALLOC <INDEX> <SIZE>
{
	int l,i;
	l=*(int *)(arena+index-4)-12;	//retine dimensiunea blocului vechi
	eliberare(arena,n,index);	//eliberarea blocului vechi
	alocare(arena,n,size,ind);	//alocarea blocului nou
	if(l>size)	//daca blocul nou are dimensiunea mai mica ca cea a blocului vechi
		for(i=0;i<size;i++)	//copie "size" octeti din blocul vechi in cel nou
			*(arena+*ind+i)=*(arena+index+i);
	else
		for(i=0;i<l;i++)
			*(arena+*ind+i)=*(arena+index+i);
}

int main()
{
	unsigned char *arena;	//arena cu care lucrez
	char s[MAX]="abc", *p;	//sirul de caractere s[100] in care retin comenzile si pointerul p in care retin cuvintele/numerele din comenzi
	int n;	//dimensiunea arenei
	ind=malloc(sizeof(int));	//alocare de memorie pentru un index pe care il voi folosi in functia care face REALLOC
	while(strcmp(s,"FINALIZE\n")!=0)	//citeste comenzi pana la comanda FINALIZE
	{  
		fgets(s,MAX,stdin);	//citire comanda
	  	p=strtok(s," ");	//selectarea primului cuvant din comanda citita
	    	if(strcmp(p,"INITIALIZE")==0)	//recunoasterea comenzii 
		{
			n=atoi(strtok(NULL," "));	//dimensiunea arenei
			arena=calloc(n,sizeof(unsigned char)); 		//alocare de memorie pentru arena si initializarea tuturor elem acesteia cu 0
			initialize(arena);
		}
		if(strcmp(p,"FINALIZE")==0 || strcmp(s,"FINALIZE\n")==0)
			finalize(arena);
		if(strcmp(p,"DUMP")==0 || strcmp(s,"DUMP\n")==0)
			dump(arena,n);	
	    	if(strcmp(p,"ALLOC")==0)
			alocare(arena,n,atoi(strtok(NULL," ")),ind);	//ind este indexul pe care il afiseaza comanda ALLOC <SIZE>
	    	if(strcmp(p,"FREE")==0)
			eliberare(arena,n,atoi(strtok(NULL," ")));
	    	if(strcmp(p,"FILL")==0)
			umplere(arena,n,atoi(strtok(NULL," ")),atoi(strtok(NULL," ")),atoi(strtok(NULL," ")));	//parametrii lui FILL sunt dati in ordine inversa in functie fata de cum s-au obtinut
	    	if(strcmp(p,"SHOW")==0)
			show(arena,n,strtok(NULL," "));	
	    	if(strcmp(p,"ALLOCALIGNED")==0)
			allocaligne(arena,n,atoi(strtok(NULL," ")),atoi(strtok(NULL," ")));
	    	if(strcmp(p,"REALLOC")==0)
			realocare(arena,n,atoi(strtok(NULL," ")),atoi(strtok(NULL," ")),ind);	
	}
	return 0;
}
