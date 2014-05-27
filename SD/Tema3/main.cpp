#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm> //pentru functia reverse care face flip pe un char *
#include"bitmap.h"
#include"minheap.h"
#include"Huffman.h"
using namespace std; //pentru functia reverse

//afisez usage-ul programului
void printUsage1() 
{
	fprintf(stderr,"Program usage:\n");
	fprintf(stderr,"\t./mycompbmp -c <file1.bmp> [<file2.mcb>]\n");
	fprintf(stderr,"\t\tor\n");
	fprintf(stderr,"\t./mycompbmp -d <file1.mcb> [<file2.bmp>]\n");	
	fprintf(stderr,"\n");
}

void printUsage2(FILE *bmpFile,FILE *mcbFile)
{
	if(bmpFile!=NULL)
	{
		fclose(bmpFile);
		fclose(mcbFile);
	}
	fprintf(stderr,"Program usage:\n");
	fprintf(stderr,"\tERROR: Invalid Bitmap format!\n");	
	fprintf(stderr,"\n");
}

void printUsage3(FILE *mcbFile,FILE *bmpFile)
{
	if(mcbFile!=NULL)
	{
		fclose(mcbFile);
		fclose(bmpFile);
	}
	fprintf(stderr,"Program usage:\n");
	fprintf(stderr,"\tERROR: Invalid My Compressed Bitmap format!\n");	
	fprintf(stderr,"\n");
}

// functie care primeste un char * de dimensiune 8 de 0 si 1 si intoarce
// valoarea sa zecimala retinuta intr-un buffer de tipul uint8_t (1 byte)
uint8_t formare(char cd[1000]) 
{
	uint8_t buffer=0;
	unsigned int j;
	for(j=0;j<strlen(cd);j++)
		buffer |= (cd[j]-'0') << (strlen(cd)-1-j);
	return buffer;
}

//functia main
int main(int argc, char **argv) 
{
	FILE *bmpFile=NULL, *mcbFile=NULL;
	BITMAPFILEHEADER bfh; //primul header bitmap 
	BITMAPINFOHEADER bih; //al doilea header bitmap
	RGBQUAD ctable[256]; //tabela de culori
	int tableSize = 0; //dimensiunea tabelei de culori(nr indicilor de culoare)
	if(strcmp(*argv,"./mycompbmp")!=0) //testam numele executabilului
	{
		printUsage1();
		exit(1);
	}
	argc--; argv++; //sarim peste executabilul programului	
	char arg1[100], arg2[100];
	strncpy(arg1,argv[argc-1]+strlen(argv[argc-1])-4,4);
	strncpy(arg2,argv[1]+strlen(argv[1])-4,4);
	//verific ceilalti parametrii ai comenzii
	if((argc!=2 && argc!=3) || (strcmp(*argv,"-c")!=0 && strcmp(*argv,"-d")!=0)
	   || (strcmp(arg1,".mcb")!=0 && argc==3 && strcmp(*argv,"-c")==0) || 
	   (argc==3 && strcmp(*argv,"-d")==0 && strcmp(arg1,".bmp")!=0))	   
	{
		printUsage1();
		exit(1);
	}
	if(strcmp(arg2,".bmp")!=0 && strcmp(*argv,"-c")==0)
	{
		printUsage2(bmpFile,mcbFile);
		exit(1);
	}
	if(strcmp(arg2,".mcb")!=0 && strcmp(*argv,"-d")==0)
	{
		printUsage3(mcbFile,bmpFile);
		exit(1);
	}
	if(strcmp(*argv,"-c")==0)
	{
		bmpFile=fopen(argv[1],"rb"); //deschidem fisierul .bmp
		if(bmpFile==NULL) 
		{
			fprintf(stderr,"ERROR: %s cannot be open!\n",argv[1]);
			exit(1);
		}
		if(argc==2)
		{
			char s[1000];
			strncpy(s,argv[1],strlen(argv[1])-4);
			strcpy(s,strcat(s,".mcb"));
			mcbFile=fopen(s,"wb");
		}
		else
			mcbFile=fopen(argv[2],"wb");
		// citesc primul header
		if(!fread(&bfh,sizeof(BITMAPFILEHEADER),1,bmpFile)) 
		{
			printUsage2(bmpFile,mcbFile);
			exit(1);
		}
		// verificam formatul ID-ului bitmap
		// cuvantul este stocat in format little endian
		if((bfh.bfType & 0x00FF)!='B' || ((bfh.bfType >> 8) & 0x00FF)!='M') 
		{
			// afisam un mesaj de eroare daca fisierul nu este un .bmp
			printUsage2(bmpFile,mcbFile);
			exit(1);
		}
		// citesc urmatorul header
		if(!fread(&bih,sizeof(BITMAPINFOHEADER),1,bmpFile)) 
		{
			printUsage2(bmpFile,mcbFile);
			exit(1);
		}
		tableSize=bih.biClrUsed;
		if(tableSize==0)
			tableSize=256;
		// citesc tabela de culori
		if(fread(ctable,sizeof(RGBQUAD),tableSize,bmpFile) < unsigned(tableSize)) 
		{
			printUsage2(bmpFile,mcbFile);
			exit(1);
		}
		// un .bmp trebuie sa aiba 8 biti de culoare (256 indici de tabel),
		// si sa nu fie comprimat, pt a fi convertit la format .mcb
		if(bih.biBitCount!=8 || bih.biClrUsed>256 || bih.biClrImportant>256 
		   || bih.biCompression!=BI_RGB) 
			{
				fprintf(stderr,"This bitmap is not suitable for .mcb format!\n");
				fclose(bmpFile);
				fclose(mcbFile);
				exit(1);
			}
		// aplic seek pana la obtinerea sirului de pixeli
		fseek(bmpFile, bfh.bfOffBits, SEEK_SET);
		// citesc sirul de biti
		uint8_t pixel;
		uint32_t pxl[256]={0}; //retin frecventa fiecarui index de culoare
		int i;
		for(i=1;i<=(bih.biWidth+4*(bih.biWidth%4!=0)-bih.biWidth%4)*bih.biHeight;i++)
		{
			if (!fread(&pixel,sizeof(uint8_t),1,bmpFile))
			{
				printUsage2(bmpFile,mcbFile);
				exit(1);
			}
			if((i-1)%(bih.biWidth+4*(bih.biWidth%4!=0)-bih.biWidth%4)<bih.biWidth
			   || bih.biWidth%4==0) // daca indexul nu face parte din padding
				pxl[pixel]++; //cresc frecventa
			}
		}
		struct tagHuffmanDerivat x,y,v[10000];
		Heap<tagHuffmanDerivat> heap(256);
		// creez un minheap de noduri de tip tagHuffmanDerivat
		for(i=0;i<256;i++)
			if(pxl[i]!=0) //daca frecventa este !=0
			{
				x.isTerminal=1;
				x.frecv=pxl[i];
				x.colorIndex=i;	
				heap.insertElement(x);
			}
		int t=0,d=heap.currentDim; //retin in d dimensiunea heapului si
								   //numarul de indexi unici
		//urmeaza algoritmul de scoatere din minheap a nodurilor si 
		//construire a unui vector de structuri de tip tagHuffmanDerivat
		while(heap.peek().frecv!=0) //cat timp heapul nu este gol
		{
			x=heap.extractMin(); //extrag primul nod
			if(heap.peek().frecv!=0)
				y=heap.extractMin(); //extrag si cel de-al doilea nod
			else
				break;
			if(x.isTerminal==1)
			{
				x.index=t;
				v[t]=x;
				t++; //indexul si de asemenea nr total de noduri din vector
			}
			if(y.isTerminal==1)
			{
				y.index=t;
				v[t]=y;
				t++;
			}		
			//construiesc urmatorul nod din vector pe care apoi il voi insera
			//inapoi in minheap	
			v[d].isTerminal=0;
			v[d].frecv=x.frecv+y.frecv;
			v[d].index=d;
			v[d].chldData.leftChld=x.index;
			v[d].chldData.rightChld=y.index;
			v[x.index].parinte=d;
			v[y.index].parinte=d;
			v[x.index].tipnod=1;
			v[y.index].tipnod=0;
			heap.insertElement(v[d]);
			d++;
		}
		// construirea fisierului .mcb din mcbFile pana la sirul de pixeli
		struct tagSerialHuffmanNode w[10000];
		struct tagcodificare z[256];		
		for(i=0;i<d;i++)
		{
			w[i].isTerminal=v[i].isTerminal; //transfer din tagHuffmanDerivat
											 //in tagserialHuffmanNode
			if(v[i].isTerminal==1)
			{
				w[i].colorIndex=v[i].colorIndex;
				z[v[i].colorIndex].frecv=v[i].frecv;
				sprintf(z[v[i].colorIndex].cod,"%3d",v[i].tipnod);
				uint16_t j=v[i].parinte; //pornesc din fiecare frunza inspre radacina
				while(j!=d-1)
				{	
					if(v[j].tipnod==0)
						strcat(z[v[i].colorIndex].cod,"0"); //formez codul binar
					else
						strcat(z[v[i].colorIndex].cod,"1");
					j=v[j].parinte; //urc un nivel in arbore
				}
			}				
			else
			{
				//retin indicii fiilor ca si cum vectorul ar fi inversat
				w[i].chldData.leftChld=d-1-v[i].chldData.leftChld; 
				w[i].chldData.rightChld=d-1-v[i].chldData.rightChld;	
			}
		}
		//inversez vectorul pentru a avea radacina pe pozitia 0
		for(int q=0,r=d-1;q!=r;q++,r--) 
		{
			w[9999] = w[q];
			w[q] = w[r];
			w[r] = w[9999];
		}
		for(i=0;i<256;i++)
			if(z[i].frecv!=0)
			{
				//inversez codul deoarece era de la frunza la radacina si nu invers
				reverse(z[i].cod,z[i].cod+strlen(z[i].cod));
				z[i].cod[strlen(z[i].cod)-2]='\0'; //in urma lui strcat am obtinut
												   //o dimensiune mai mare cu 2
			}		
		//fac scrierea in fisier
		bfh.bfType='M'+('C'<<8);
		fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,mcbFile);
		bih.biCompression=BI_MCB;
		fwrite(&bih,sizeof(BITMAPINFOHEADER),1,mcbFile);
		fwrite(ctable,sizeof(RGBQUAD),tableSize,mcbFile);
		d=(uint16_t)d;
		fwrite(&d,sizeof(uint16_t),1,mcbFile); //scriu nr de noduri Huffman
		fwrite(w,sizeof(tagSerialHuffmanNode),d,mcbFile); //scriu vectorul Huffman
		fseek(bmpFile, 0, SEEK_SET);
		fseek(bmpFile, bfh.bfOffBits, SEEK_SET); //revin la sirul de pixeli initial
		char cd[1000],trans[1000],h[1000];
		cd[0]='\0'; trans[0]='\0'; h[0]='\0';
		uint8_t b;
		for(i=1;i<=(bih.biWidth+4*(bih.biWidth%4!=0)-bih.biWidth%4)*bih.biHeight;i++)
		{
			fread(&pixel,sizeof(uint8_t),1,bmpFile); //pe masura ce citesc index cu index
			if((i-1)%(bih.biWidth+4*(bih.biWidth%4!=0)-bih.biWidth%4)<bih.biWidth
			   || bih.biWidth%4==0) //daca nu face parte din padding
			{
				if(strlen(cd)<8) //daca nu am un octet complet
					strcat(cd,z[pixel].cod); //adaug urmatorul cod
				if(strlen(cd)==8) //daca s-a completat perfect un octet
				{		
					reverse(cd,cd+strlen(cd)); //inversez codul pt scriere in fisier
					b=formare(cd);
					fwrite(&b,sizeof(uint8_t),1,mcbFile);
					cd[0]='\0';
				}
				if(strlen(cd)>8) //daca s-a completat mai mult de un octet
				{
					trans[0]='\0';
					strncpy(trans,cd+8,strlen(cd)-7); //retin surplusul de biti
					strncpy(h,cd,8);
					strcpy(cd,h);
					reverse(cd,cd+strlen(cd));
					b=formare(cd);
					fwrite(&b,sizeof(uint8_t),1,mcbFile); //afisez octetul fara surplus
					while(strlen(trans)>=8) //daca am un surplus care depaseste un octet
					{
						strncpy(trans,cd+8,strlen(cd)-7); //retin un octet din surplus
						strncpy(h,cd,8);
						strcpy(cd,h);
						reverse(cd,cd+strlen(cd));
						b=formare(cd);
						fwrite(&b,sizeof(uint8_t),1,mcbFile);
					}
					strcpy(cd,trans); //adaug surplusul la urmatorul octet (cod)
				}
			}
		}
		if(strlen(trans)>0) //daca la sfarsit mi-a mai ramas necompletat un octet,
							//ii adaug un padding
		{
			unsigned int j,t=strlen(trans);
			for(j=0;j<8-t;j++)
				strcat(trans,"0");
			reverse(trans,trans+strlen(trans));
			b=formare(trans);
			fwrite(&b,sizeof(uint8_t),1,mcbFile);
		}			
	}
	else
	{
		mcbFile=fopen(argv[1],"rb"); //deschidem fisierul .mcb
		if(mcbFile==NULL) 
		{
			fprintf(stderr,"ERROR: %s cannot be open!\n",argv[1]);
			exit(1);
		}
		if(argc==2)
		{
			char s[1000];
			strncpy(s,argv[1],strlen(argv[1])-4);
			strcpy(s,strcat(s,".bmp"));
			bmpFile=fopen(s,"wb");
		}
		else
			bmpFile=fopen(argv[2],"wb");
		// citesc primul header
		if(!fread(&bfh,sizeof(BITMAPFILEHEADER),1,mcbFile)) 
		{
			printUsage3(mcbFile,bmpFile);
			exit(1);
		}
		// verificam formatul ID-ului mcb
		// cuvantul este stocat in format little endian
		if((bfh.bfType & 0x00FF)!='M' || ((bfh.bfType >> 8) & 0x00FF)!='C') 
		{
			// afisam un mesaj de eroare daca fisierul nu este un .mcb
			printUsage3(mcbFile,bmpFile);
			exit(1);
		}
		// citesc urmatorul header
		if(!fread(&bih,sizeof(BITMAPINFOHEADER),1,mcbFile)) 
		{
			printUsage3(mcbFile,bmpFile);
			exit(1);
		}
		tableSize=bih.biClrUsed;
		if(tableSize==0)
			tableSize=256;
		// citesc tabela de culori
		if(fread(ctable,sizeof(RGBQUAD),tableSize,mcbFile) < unsigned(tableSize)) 
		{
			printUsage3(mcbFile,bmpFile);
			printf("culori\n");
			exit(1);
		}
		// un .mcb trebuie sa aiba 8 biti de culoare (256 indici de tabel),
		// si sa fie comprimat, pt a fi convertit la format .bmp
		if(bih.biBitCount!=8 || bih.biClrUsed>256 || bih.biClrImportant>256 
		   || bih.biCompression!=BI_MCB) 
			{
				fprintf(stderr,"This bitmap is not suitable for .bmp format!\n");
				fclose(mcbFile);
				fclose(bmpFile);
				exit(1);
			}
		// construirea fisierului .bmp din bmpFile pana la sirul de pixeli
		bfh.bfType='B'+('M'<<8);
		fwrite(&bfh,sizeof(BITMAPFILEHEADER),1,bmpFile);
		bih.biCompression=BI_RGB;
		fwrite(&bih,sizeof(BITMAPINFOHEADER),1,bmpFile);
		fwrite(ctable,sizeof(RGBQUAD),tableSize,bmpFile);
		// aplic seek pana la obtinerea sirului de pixeli
		fseek(mcbFile, bfh.bfOffBits, SEEK_SET);
		// citesc sirul de biti
		uint16_t d;
		struct tagSerialHuffmanNode v[10000];
		fread(&d,sizeof(uint16_t),1,mcbFile); //citesc nr de noduri Huffman
		fread(v,sizeof(tagSerialHuffmanNode),d,mcbFile); //citesc vectorul Huffman
		int i;
		for(i=0;i<d;i++)
			if(v[i].isTerminal==0)
			{	//modific indicii fiilor nodurilor ca pentru un vector inversat
				v[i].chldData.leftChld=d-1-v[i].chldData.leftChld;
				v[i].chldData.rightChld=d-1-v[i].chldData.rightChld;
			}
		//inversez vectorul Huffman pentru a avea radacina pe ultima pozitie
		for(int q=0, r =d-1; q!=r; q++,r--) 
		{
			v[9999] = v[q];
			v[q] = v[r];
			v[r] = v[9999];
		}
		uint8_t pixel,y;
		i=d-1; //pornesc de pe pozitia radacinii
		int k,j=0,l;
		while(1)
		{
			if (!fread(&pixel,sizeof(uint8_t),1,mcbFile)) //pe masura ca citesc un elem
				break;
			k=0; //in k retin cati biti am parcurs din octetul curent
			if(pixel==0) //daca am ajuns la ultimul octet si acesta e 0
				while(j%(bih.biWidth+4*(bih.biWidth%4!=0)-bih.biWidth%4)!=0)
				{   //cat timp mai trebuie sa scriu in fisier indexi de culoare
					if(v[i].isTerminal==1)
					{
						fwrite(&v[i].colorIndex,sizeof(uint8_t),1,bmpFile);
						i=d-1; //revin in radacina
						j++; //cresc numarul de indexi afisati
						if(j%(bih.biWidth+4*(bih.biWidth%4!=0)-bih.biWidth
						   %4)>=bih.biWidth && bih.biWidth%4!=0)
						{
							for(l=0;l<4*(bih.biWidth%4!=0)-
							    bih.biWidth%4;l++)
							{
								uint8_t g=0; //adaug padding daca e necesar
								fwrite(&g,sizeof(uint8_t),1,bmpFile);
							}
							j+=4*(bih.biWidth%4!=0)-bih.biWidth%4;
						}						
					}
					else
						i=v[i].chldData.rightChld; //parcurg fii drepti (0)
				}
			y=pixel; //salvez pixel
			while(y) //urmeaza convertirea lui pixel din zecimal in binar
			{
				if(v[i].isTerminal==1)
				{
					fwrite(&v[i].colorIndex,sizeof(uint8_t),1,bmpFile);
					i=d-1; //revin in radacina
					j++; //cresc numarul de indexi afisati
					if(j%(bih.biWidth+4*(bih.biWidth%4!=0)-bih.biWidth%4)>=
					   bih.biWidth && bih.biWidth%4!=0)
					{
						for(l=0;l<4*(bih.biWidth%4!=0)-bih.biWidth%4;l++)
						{
							uint8_t g=0; //adaug padding daca e necesar
							fwrite(&g,sizeof(uint8_t),1,bmpFile);
						}
						j+=4*(bih.biWidth%4!=0)-bih.biWidth%4;
					}
				}
				else
				{
					if(y%2==1)
						i=v[i].chldData.leftChld; //parcurg de la rad spre frunza
					else
						i=v[i].chldData.rightChld;
					y/=2;
					k++; //creste numarul de biti parcursi din octetul curent
				}
			}
			while(k<8 && pixel!=0) //daca mai am de parcurs biti din octetul curent
			{ 					   //inseamna ca mai am de parcurs biti de 0
				if(v[i].isTerminal==1)
				{
					fwrite(&v[i].colorIndex,sizeof(uint8_t),1,bmpFile);
					i=d-1; //revin in radacina
					j++; //cresc numarul de indexi afisati
					if(j%(bih.biWidth+4*(bih.biWidth%4!=0)-bih.biWidth%4)>=
					   bih.biWidth && bih.biWidth%4!=0)
					{
						for(l=0;l<4*(bih.biWidth%4!=0)-bih.biWidth%4;l++)
						{
							uint8_t g=0; //adaug padding daca e necesar
							fwrite(&g,sizeof(uint8_t),1,bmpFile);
						}					
						j+=4*(bih.biWidth%4!=0)-bih.biWidth%4;
					}
				}
				else
				{
					i=v[i].chldData.rightChld; //parcurg fii drepti (0)		
					k++; //creste numarul de biti parcursi din octetul curent
				}
			}
		}
	}	
	//inchid toate fisierele	
	fclose(bmpFile);
	fclose(mcbFile);
	return 0;
}
