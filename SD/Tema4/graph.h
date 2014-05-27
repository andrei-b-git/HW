#include <stdio.h>
#include "procesor.h"

template<typename T> class Graph 
{
    public:
        int N; //numarul de noduri/taskuri din graph
        LinkedList<int> *L; //lista de adiacenta
        T *timp; //timpul asociat fiecarui nod/task
		int *status; //statusul fiecarui task poate lua 3 valori:
					 //0=neprocesat, 1=in procesare, 2=procesat

        Graph(int numNodes) //constructorul primeste ca parametru
		{					//numarul de taskuri
            N=numNodes;
            L=new LinkedList<int> [N+1]; //alocarea listei 
            timp=new T[N+1]; //alocarea vectorului de timp
			status=new int[N+1]; //alocarea vectorului de status
			int i;
			for(i=1;i<=N;i++)
				status[i]=0; //la inceput toate taskurile sunt neprocesate
        }

        void setNodeInfo(int i,T info) //setter al timpului unui task
		{
            timp[i]=info;
        }

        T getNodeInfo(int i) //getter al timpului unui task
		{
            return timp[i];
        }
		
		void setNodeStatus(int i,int st) //setter al statusului unui task
		{
			status[i]=st;
		}		

		int getNodeStatus(int i) //getter al statusului unui task
		{
			return status[i];
		}

        void addEdge(int i, int j) //metoda de constructie a listei de
		{	//adiacenta pt un nod cu mentiunea ca retine muchiile orientate
			//in sens invers
            L[j].addFirst(i);
        }

		int prepared(int i) //metoda de precizare daca un task are toate 
		{					//dependentele procesate sau n-are deloc dependente
			if(L[i].isEmpty()==1) //daca n-are dependente
				return 1;
			struct list_elem<int> *p;
			p=L[i].pfirst;
			int o=1;
			while(p!=NULL) //parcurgerea listei de adiacenta
			{
				if(status[p->info]!=2) //daca dependenta nu este procesata
					o=0;
				p=p->next;
			}
			if(o==1) //daca nu s-a gasit vreo dependenta neprocesata
				return 1;
			return 0;
		}

		void planificare(int M)
		{
			int i,finished=0, t=0, k=0, j=0; //finished retine numarul de taskuri
			//terminate de procesat, t este contorul general de timp, k retine
			//indicele primului procesor liber, j cate planificari s-au facut la 
			//momentul de timp actual
			Procesor<int> pr(M);
			while(finished<N) //cat timp am taskuri neprocesate
			{
				for(i=1;i<=M;i++) //parcurg toate procesoarele
					if(pr.getProcStatus(i)!=0) //daca procesorul nu e liber
					{	
						if(pr.getProcTexe(i)==getNodeInfo(pr.getProcStatus(i)))
						//daca timpul de executie este egal cu cel al taskului de
						//pe procesor
						{
							setNodeStatus(pr.getProcStatus(i),2); //taskul de pe 
							//procesor devine procesat
							pr.setProcStatus(i,0); //procesorul devine liber
							pr.setProcTexe(i,0); //resetam timpul de executie la 0
						}
						else
							pr.setProcTexe(i,pr.getProcTexe(i)+1); //crestem timpul
																   //de executie
					}
				for(i=1;i<=N;i++) //parcurgem toate taskurile
					if(getNodeStatus(i)==0 && prepared(i)==1 && pr.FirstFreeProc()!=0)
					//daca taskul e neprocesat, n-are dependente neprocesate si exista
					//procesor liber
					{
						if(j==0) //daca suntem intr-un moment de timp nou
							printf("T %d\n",t); //afisam momentul de timp actual
						j++; //creste numarul de planificari dintr-un moment de timp
						k=pr.FirstFreeProc(); //retinem primul procesor liber
						printf("%d %d\n",k,i); //afisam procesorul si taskul
						pr.setProcStatus(k,i); //procesorul ia taskul ca status
						pr.setProcTexe(k,pr.getProcTexe(k)+1); //crestem timpul de exec
						setNodeStatus(i,1); //taskul curent este acum in procesare
						finished++; //numarul de taskuri procesate creste
					}
				j=0; //resetam numarul de planificari dintr-un moment de timp
				t++; //contorul de timp creste
			}
		}

        ~Graph() //destructorul
		{
            int i;
            delete timp; //dealocarea vectorului de timp
			delete status; //dealocarea vectorului de status
            for(i=1;i<=N;i++) 
                while(!L[i].isEmpty())
                    L[i].removeFirst(); //dealocarea listei de adiacenta
            delete L;
        }
};
