#include <stdio.h>

template<typename T> class Procesor 
{
    public:
		int M; //numarul de procesoare de pe sistem
		int *status; //statusul asociat fiecarui procesor
		T *texe; //timpul de executie pana la momentul actual
				 //al taskului de pe fiecare procesor

		Procesor(int nrProc) //constructorul primeste ca parametru
		{					 //numarul de procesoare
			M=nrProc;
			status=new int[M+1]; //alocarea vectorului de status
			texe=new T[M+1]; //alocarea vectorului de timp de executie
			int i;
			for(i=1;i<=M;i++)
			{
				status[i]=0; //initial toate procesoarele sunt libere
				texe[i]=0; //initial timpul de executie e 0
			}
		}

		void setProcStatus(int i,int st) //setter al statusului unui procesor
		{
			status[i]=st;
		}

		int getProcStatus(int i) //getter al statusului unui procesor
		{
			return status[i];
		}

		void setProcTexe(int i,T timp) //setter al timpului de executie
		{
			texe[i]=timp;
		}

		T getProcTexe(int i) //getter al timpului de executie
		{
			return texe[i];
		}

		int FirstFreeProc() //metoda ce returneaza primul procesor liber sau
		{					//0 daca toate procesoarele sunt ocupate
			int i;
			for(i=1;i<=M;i++)
				if(status[i]==0) //daca procesorul este liber
					return i;
			return 0;
		}

		~Procesor() //destructorul
		{
			delete status; //dealocarea vectorului de status
			delete texe; //dealocarea vectorului de timp de executie
		}
};
