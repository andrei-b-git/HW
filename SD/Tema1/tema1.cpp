/*
# nume: Bojin
# prenume: Andrei Florian
# grupa/serie: 313 CA
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

template<typename T,typename K,int N> struct comanda
{	
	K s[N];	//string care ajuta la recunoastere comenzii, de ex: NEW
	T tmpa, tmpe, prio, ind; /*parametrii comenzii: timp de aparitie, timp de 
	executie al procesului, prioritatea procesului, indicele cozii waiting*/
};

template<typename T> struct proces
{
	T pid,tmpe,prio; /*caracteristicile fiecarui proces: PID, timp de
			   executie, prioritate*/ 
};

template <typename T,int N> class Queue	//clasa coada circulara
{
	private:
		int head;
		int tail;
		int size;
		T queueArray[N];
	public:
		Queue()	//constructorul
		{
			head=tail=size=0;
		}
		~Queue() //destructorul
		{
		}
		void enqueue(T x)
		{
			if(size==N)
				fprintf(stderr,"Error 101: The queue is full!\n");
			else
			{
				queueArray[tail]=x;
				tail=(tail+1)%N;
				size++;
			}
		}
		T dequeue()
		{
			if(isEmpty())
			{
				fprintf(stderr,"Error 102: The queue is empty!\n");
				T x;
				return x;
			}
			else
			{
				T x=queueArray[head];
				head=(head+1)%N;
				size--;
				return x;
			}
		}
		T peek()
		{
			if(isEmpty())
			{
				fprintf(stderr,"Error 103: The queue is empty!\n");
				T x;
				return x;
			}
			else
				return queueArray[head];
		}
		int isEmpty()
		{
			return (size==0);
		}
};
	
int readyempty(Queue<struct proces<int>,100> ready[5]) /*functie de testare a 
 continutului fiecarei cozi ready si returnare a primei cozi nevide gasita*/
{
	int i;
	for(i=0;i<=4;i++)
		if(ready[i].isEmpty()!=1)
			return i;
	return -1;
}

int main()
{
	Queue<struct proces<int>,100> ready[5], wait[3], aux; /*vectorii de cozi
 	ready, waiting si coada aux necesara in cadrul interpretarii comenzii SIGNAL,
	toti lucrand cu structuri de tip proces*/
	Queue<struct comanda<int,char,100>, 100> cmd; /*coada cmd in care retin 
	comenzile ca structuri, odata ce au fost separate in parametrii*/
	char sir[100], *p; //sirul in care fac citirea si pointerul p pt strtok
	comanda<int,char,100> c;
	gets(sir);
	while(strcmp(sir,"FINALIZE")!=0) //citirea comenzilor pana la FINALIZE
	{
		int n=0; //numarul parametrilor fiecarei comenzi
		p=strtok(sir," "); //separarea fiecarei comenzi in parametrii
		while(p!=NULL)
		{
			if(n==0)
			{
				if(strcmp(p,"NEW")==0)
				{
					strcpy(c.s,p); /*retinerea fiecarui 
					parametru in structura de tip comanda c*/
					c.ind=-1; /*inlocuirea cu -1 a parametrilor
					nefolositori in functie de comanda*/
				}
				if(strcmp(p,"WAIT")==0 || strcmp(p,"SIGNAL")==0)
				{
					strcpy(c.s,p);
					c.tmpe=-1;
					c.prio=-1;
				}
				n++; //trecerea la urmatorul parametru
			}
			else
				if(n==1)
				{
					c.tmpa=atoi(p);
					n++;
				}
				else
					if(n==2)
					{
						if(strcmp(c.s,"NEW")==0)
							c.tmpe=atoi(p);
						if(strcmp(c.s,"WAIT")==0 || 
						   strcmp(c.s,"SIGNAL")==0)
						{
							c.ind=atoi(p);
							break;
						}
						n++;
					}
					else
						if(n==3)
						{
							c.prio=atoi(p);
							break;
						}		
			p=strtok(NULL," ");
		}
		cmd.enqueue(c); //retinerea fiecarei comenzi in coada de comenzi cmd
		gets(sir); //citirea urmatoarei comenzi neprocesate
	}
	int timp=0, id=0, pas=0; /*unitatea de timp, PIDul procesului si
	contorul unitatilor de timp cate sta un proces in executie*/
	int size, i; //variabile pt retinerea lungimii si parcurgerea cozii ready
	proces<int> running, pc; //running retine procesul in executie
	running.pid=-1; //un PID=-1 indica faptul ca nu exista vreun proces in executie
	while(cmd.isEmpty()!=1 || running.pid!=-1 || readyempty(ready)!=-1) 
	//cat timp exista comenzi de interpretat si sunt procese in executie sau de executat
	{
		if(cmd.isEmpty()!=1) //daca exista comenzi de interpretat
		{
			c=cmd.peek();
			if(c.tmpa+1==timp && strcmp(c.s,"NEW")!=0) /*daca (timpul de aparitie 
			al comenzii)+1 coincide cu timpul actual si comanda NU e NEW*/
			
			{
				c=cmd.dequeue(); //scoate comanda din coada de comenzi
				if(strcmp(c.s,"WAIT")==0) //daca comanda e WAIT
				{
					wait[c.ind].enqueue(running);
					running.pid=-1; /*scoate din executie procesul curent
					si il duce in coada wait cu indicele c.ind*/
				}	
				if(strcmp(c.s,"SIGNAL")==0) //daca comanda e SIGNAL
					while(wait[c.ind].isEmpty()!=1)	/*cat timp coada wait
					cu indicele c.ind nu e goala*/
					{
						pc=wait[c.ind].dequeue(); //retine procesul
						if(pc.prio==0 && ready[0].isEmpty()!=1)
					//daca prioritatea e 0 si mai am procese in ready[0]
						{
							size=0; i=0;
							while(ready[0].isEmpty()!=1)
							{
								aux.enqueue(ready[0].dequeue());
								//transfer toate procesele in aux
								size++; //calc lungimea ready[0]
							}
							while(i<size-1)
							{
								ready[0].enqueue(aux.dequeue());
								//transfer size-1 procese din aux
								i++;
							}
							ready[0].enqueue(pc); //transfer procesul
							ready[0].enqueue(aux.dequeue());
							//transfer ultimul proces din ready[0]						
						}
						else
							if(pc.prio==0)
								ready[pc.prio].enqueue(pc);
								//transfera procesul in ready
							else
							{
								pc.prio-=1; //mareste prioritatea
								ready[pc.prio].enqueue(pc);
								//transfera procesul in ready
							}
					}
			}
		}		
		if(running.pid==-1) //daca nu exista procese in executie
		{
			if(readyempty(ready)==-1 && cmd.isEmpty()!=1) 
			//daca nu exista alte procese in asteptare dar exista comenzi
	   			printf("[%d] NOP\n",timp); //nu se executa nimic
			else
				if(readyempty(ready)!=-1) /*daca exista alte procese
							    in asteptare*/
				{
					running=ready[readyempty(ready)].dequeue();
				//pune in executie primul proces convenabil ca prioritate
					pas=0;	
				}
		}
		if(cmd.isEmpty()!=1) //daca exista comenzi de interpretat
		{
			c=cmd.peek();
			if(c.tmpa==timp && strcmp(c.s,"NEW")==0) /*daca timpul de 
			aparitie al comenzii coincide cu timpul actual si comanda e NEW*/
			{
				c=cmd.dequeue(); //scoate comanda din coada de comenzi
				id++; //creaza un nou PID
				pc.pid=id;
				pc.tmpe=c.tmpe; //retine parametrii comenzii intrun proces
				pc.prio=c.prio;
				ready[pc.prio].enqueue(pc); /*retine procesul nou creat in
				coada ready corespunzatoare ca prioritate*/
			}
		}
		if(running.pid!=-1) //daca exista procese in executie
		{
			printf("[%d] PID %d\n",timp,running.pid); /*afiseaza procesul
			curent aflat in executie impreuna cu timpul asociat*/ 
			running.tmpe--; //decrementeaza timpul de executie ramas
			pas++; //creste contorul timpului cat sta un proces in executie
			if(running.tmpe==0)
				running.pid=-1; /*scoate din executie procesul daca si-a 
				terminat de executat timpul*/
			if(pas==3 && running.tmpe!=0) /*daca procesul a ajuns la 3 executari
			consecutive si mai are timp de executie ramas*/
			{
				ready[running.prio].enqueue(running);
				running.pid=-1; //scoate din executie procesul in coada ready
			}
		}
		timp++; //incrementeaza timpul actual cu o unitate de timp
	}
	return 0;
}
