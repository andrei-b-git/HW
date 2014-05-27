/*
# nume: Bojin
# prenume: Andrei Florian
# grupa/seria: 313 CA
*/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

char *p;	//in p retin sirul prefix	
int id=-1, ids=-1;	//id=numarul de matrici dinamice / ids=numarul de submatrici
FILE *fp[10000];	//in fp retin fiecare fisier 

struct matrice
{
	int nl;	//numarul de linii al matricii
	int nc;	//numarul de coloane al matricii
	int **mat;	//matricea dinamica in sine
} m[10000];	//dimensiune este reprezentata de numarul de matrici

struct submatrice
{
	int sr;	//start_row
	int er;	//end_row
	int sc;	//start_col
	int ec;	//end_col
	int **sm;	//submatricea
	int idp;	//id-ul matricei dinamice din care provine
} w[10000];	//dimensiune este reprezentata de numarul de submatrici

void Init(char* prefix)
{
	int i;
	p=malloc((strlen(prefix)+1)*sizeof(char));	//aloc memorie pt p
	strcpy(p,prefix);
	id=-1;
	char s[100];
	sprintf(s,"%s%d",p,id+1);	//obtin concatenarea de forma "prefixid"
	fp[id+1]=fopen(s,"rb");	//deschid fisierul cu id-ul 0
	while(fp[id+1]!=NULL)	//daca acesta exista
	{
		id++;
		fread(&m[id].nl,sizeof(int),1,fp[id]);	//citesc numarul de linii
		fread(&m[id].nc,sizeof(int),1,fp[id]);	//citesc numarul de coloane
		m[id].mat=malloc(m[id].nl*sizeof(int *));
		for(i=0;i<m[id].nl;i++)
		{
			*(m[id].mat+i)=calloc(m[id].nc,sizeof(int));	//aloc matricea si o citesc din fisier
			fread(*(m[id].mat+i),sizeof(int),m[id].nc,fp[id]);
		}	
		sprintf(s,"%s%d",p,id+1);	//trec la urmatorul fisier
		fp[id+1]=fopen(s,"rb");
	}
	if(id!=-1)	//daca nu exista nici un fisier
		for(i=0;i<=id;i++)
		{
			fclose(fp[i]);	//inchid toate fisierele
			char s[100];
			sprintf(s,"%s%d",p,i);
			fp[i]=fopen(s,"wb");	//le deschid in modul wb
		}	
}

int CreateMatrix()
{
	id++;	//incrementez nr de matrici
	m[id].nl=0;	
	m[id].nc=0;
	char s[100];
	sprintf(s,"%s%d",p,id);
	fp[id]=fopen(s,"wb");	
	return id;
}

void GetMatrixDimensions(int id, int* nrows, int* ncols)
{
	*nrows=m[id].nl;
	*ncols=m[id].nc;
}

void AppendRow(int id, int* elems)
{
	int i;
	if(m[id].nc==0)	//daca nr de coloane e 0
		m[id].nl++;	//incrementez nr de linii si atat
	else
	{	
		m[id].nl++;
		if(elems!=NULL)
		{
			if(m[id].nl==1)	//daca nr de linii initial era 0 
			{
				m[id].mat=malloc(m[id].nl*sizeof(int *));
				*(m[id].mat)=calloc(m[id].nc,sizeof(int));	//aloc matricea
				for(i=0;i<m[id].nc;i++)
					*(*(m[id].mat)+i)=*(elems+i);	//copii vectorul elems pe linia noua
			}
			else
			{
				m[id].mat=realloc(m[id].mat,m[id].nl*sizeof(int *));	//realoc nr de linii
				*(m[id].mat+m[id].nl-1)=calloc(m[id].nc,sizeof(int));
				for(i=0;i<m[id].nc;i++)
					*(*(m[id].mat+m[id].nl-1)+i)=*(elems+i);	//copii vectorul elems pe ultima linie
			}
		}
	}	
}

void AppendColumn(int id, int* elems)
{
	if(m[id].nl==0)	//daca nr de linii e 0
		m[id].nc++;	//incrementez nr de coloane si atat
	else
	{	
		int i;
		m[id].nc++;
		if(elems!=NULL)
		{
			if(m[id].nc==1)	//daca nr de coloane initial era 0
			{
				m[id].mat=malloc(m[id].nl*sizeof(int *));	//aloc matricea
				for(i=0;i<m[id].nl;i++)
				{
					*(m[id].mat+i)=calloc(m[id].nc,sizeof(int));
					*(*(m[id].mat+i))=*(elems+i);	//copii vectorul elems pe coloana noua
				}
			}
			else
			{
				for(i=0;i<m[id].nl;i++)
				{
					*(m[id].mat+i)=realloc(*(m[id].mat+i),m[id].nc*sizeof(int));	//realoc nr de coloane
					*(*(m[id].mat+i)+m[id].nc-1)=*(elems+i);	//copii vectorul elems pe ultima coloana
				}
			}			
		}
	}	
}

int** GetSubMatrix1(int id, int start_row, int start_col, int end_row, int end_col)
{
	int i,j;
	ids++;
	w[ids].sm=malloc((end_row-start_row+1)*sizeof(int *));	//aloc submatricea
	for(i=0;i<(end_row-start_row+1);i++)
		*(w[ids].sm+i)=calloc((end_col-start_col+1),sizeof(int));
	for(i=start_row;i<=end_row;i++)
		for(j=start_col;j<=end_col;j++)
			*(*(w[ids].sm+i-start_row)+j-start_col)=*(*(m[id].mat+i)+j);	//obtin submatricea decupata din cea dinamica
	w[ids].sr=start_row;	
	w[ids].er=end_row;
	w[ids].sc=start_col;	//retin pozitia submatricii in matricea dinamica
	w[ids].ec=end_col;
	w[ids].idp=id;	//retin id-ul matricii dinamice din care provine submatricea
	return w[ids].sm;
}

int** GetSubMatrix2(int** base_submatrix, int start_row, int start_col, int end_row, int end_col)
{
	int i;
	int **sm;	//submatricea
	sm=(int**)malloc((end_row-start_row+1)*sizeof(int *));	//aloc submatricea
	for(i=0;i<(end_row-start_row+1);i++)	
		*(sm+i)=*(base_submatrix+i+start_row)+start_col;	//obtin submatricea decupata din submatricea initiala
	return sm;
}

void PutSubMatrix(int** submatrix)
{
	int i,k,j,l;
	for(i=0;i<=ids;i++)	//parcurg toate submatricile retinute in memorie
		if(w[i].sm==submatrix)	//compar cu submatricea data ca parametru
		{		
			k=w[i].idp;	//retin id-ul matricii dinamice initiale
			for(l=w[i].sr;l<=w[i].er;l++)
				for(j=w[i].sc;j<=w[i].ec;j++)	//in pozitiile respective
					*(*(m[k].mat+l)+j)=*(*(w[i].sm+l-w[i].sr)+j-w[i].sc);	//actualizez matricea dinamica			
			break;
		}	
}

void ExtendRight(int id1, int id2)
{
	if(m[id1].nl!=m[id2].nl)	//daca nr de linii al celor 2 matrici nu coincide nu are rost sa fac ceva
		return;
	else
	{
		int i,j;
		m[id1].nc+=m[id2].nc;	//maresc nr de coloane al primei matrici
		for(i=0;i<m[id1].nl;i++)
		{
			*(m[id1].mat+i)=realloc(*(m[id1].mat+i),m[id1].nc*sizeof(int));	//realoc nr de coloane al primei matrici
			if(id1!=id2)	//daca cele 2 matrici au id-uri diferite
			{
				for(j=m[id1].nc-m[id2].nc;j<m[id1].nc;j++)
					*(*(m[id1].mat+i)+j)=*(*(m[id2].mat+i)+j-m[id1].nc+m[id2].nc);	//trec matricea a 2a
//in dreapta primei matrici
			}
			else	//daca au acelasi id (dublez prima matrice)
			{
				for(j=m[id1].nc/2;j<m[id1].nc;j++)
					*(*(m[id1].mat+i)+j)=*(*(m[id1].mat+i)+j-m[id1].nc/2);	//trec matricea a 2a in dreapta 
//primei matrici		
			}
		}		
	}
}

void ExtendDown(int id1, int id2)
{
	if(m[id1].nc!=m[id2].nc)	//daca nr de coloane al celor 2 matrici difera nu fac nimic
		return;
	else
	{
		int i,j;
		m[id1].nl+=m[id2].nl;	//maresc nr de linii al primei matrici
		m[id1].mat=realloc(m[id1].mat,m[id1].nl*sizeof(int *));	//realoc prima matrice pe linii
		if(id1!=id2)	//daca id-urile celor doua matrici nu coincid
		{
			for(i=m[id1].nl-m[id2].nl;i<m[id1].nl;i++)
			{
				*(m[id1].mat+i)=malloc(m[id1].nc*sizeof(int));	//aloc noile linii
				for(j=0;j<m[id1].nc;j++)
					*(*(m[id1].mat+i)+j)=*(*(m[id2].mat+i-m[id1].nl+m[id2].nl)+j);	//extind in jos cu a 2a matrice
			}
		}	
		else	//daca extind prima matrice cu ea insasi
		{
			for(i=m[id1].nl/2;i<m[id1].nl;i++)
			{
				*(m[id1].mat+i)=malloc(m[id1].nc*sizeof(int));	//aloc noile linii
				for(j=0;j<m[id1].nc;j++)
					*(*(m[id1].mat+i)+j)=*(*(m[id1].mat+i-m[id1].nl/2)+j);	//extind in jos cu propria matrice
			}
		}			
	}
}

int GetNumberOfElementsBetweenValues(int id, int vmin, int vmax)
{
	int i,j,n=0;
	for(i=0;i<m[id].nl;i++)	//parcurg matricea element cu element
		for(j=0;j<m[id].nc;j++)
			if(*(*(m[id].mat+i)+j)<=vmax && *(*(m[id].mat+i)+j)>=vmin)	//testez daca e in interval
				n++;	//cresc contorul
	return n;	
}

void Finish()
{
	int i,j;
	for(i=0;i<=id;i++)	//parcurg toate matricile
	{
		fwrite(&m[i].nl,sizeof(int),1,fp[i]);	//scriu in fisier nr de linii
		fwrite(&m[i].nc,sizeof(int),1,fp[i]);	//scriu	in fisier nr de coloane
		for(j=0;j<m[i].nl;j++)
		{
			fwrite(*(m[i].mat+j),sizeof(int),m[i].nc,fp[i]);	//scriu matricea in fisier
			free(*(m[i].mat+j));	//eliberez memoria liniilor matricii
		}
		free(m[i].mat);	//eliberez memoria matricii
		fclose(fp[i]);	//inchid fisierul
	}
	for(i=0;i<=ids;i++)	//parcurg toate submatricile
	{
		for(j=0;j<(w[i].er-w[i].sr+1);j++)
	 		free(*(w[i].sm+j));	//eliberez memoria liniilor submatricii
		free(w[i].sm);	//eliberez memoria submatricii
	}
	free(p);	//eliberez memoria var care retine prefixul
}
