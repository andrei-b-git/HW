#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<cstdlib>
#include<ctime>

template<typename T,typename K,int N> struct list_elem 
{
	K token[N];
	T fv,nl;
	struct list_elem<T,K,N> **lay;
};

template<typename T,typename K,int N> class Skiplist 
{
 public:
	struct list_elem<T,K,N> *pfirst;

        void insert(K x[N],T a) 
	{
        	struct list_elem<T,K,N> *paux;
		paux=cautare(x);
		if(paux!=NULL)
			paux->fv++;
		else
		{
			paux = new struct list_elem<T,K,N>;
			paux->lay= new struct list_elem<T,K,N>*[a]; 
		        strcpy(paux->token,x);
			paux->fv=1;
			paux->nl=a;
			if(paux->nl > pfirst->nl)
			{ 
				struct list_elem<T,K,N> **v=new struct list_elem<T,K,N>*[paux->nl];
				memcpy(*v,pfirst->lay,pfirst->nl*sizeof(struct list_elem<T,K,N> *));
				int i;
				for(i=pfirst->nl+1;i<=paux->nl;i++)
				{
					pfirst->lay[i-1]=paux;
					paux->lay[i-1]=NULL;
				}
				pfirst->nl=paux->nl;
				delete [] pfirst->lay;
				*pfirst->lay=*v;
				delete [] *v;
			} 
       			struct list_elem<T,K,N> *p;
			p=pfirst;
			int i=1;
			if(isEmpty()!=1)
				while (i<=a) 
				{
					while(p->lay[a-i]!=NULL && strcmp(p->lay[a-i]->token,x)<0)
						p=p->lay[a-i];
					paux->lay[a-i]=p->lay[a-i];
					p->lay[a-i]=paux;
					i++;
		        	}
			else
			{
				paux->lay[0]=NULL;
				p->lay[0]=paux;
			}			
		}
        }

        struct list_elem<T,K,N>* cautare(K x[N]) 
	{
		if(isEmpty()!=1)
		{
        		struct list_elem<T,K,N> *paux;
			paux=pfirst;
			int i=1;
			T NL=paux->nl;
		        while (i<=NL) 
			{
				while(paux->lay[NL-i]!=NULL && strcmp(paux->lay[NL-i]->token,x)<=0)
					paux=paux->lay[NL-i];
				if(strcmp(x,paux->token)==0)
		        		return paux;
				else
					i++;
                	}
		}
            	return NULL;
	}
	
	void remove(K x[N])
	{
        	struct list_elem<T,K,N> *paux;
		paux=cautare(x);
		if(paux!=NULL)
		{
			int a=paux->nl;
       			struct list_elem<T,K,N> *p;
			p=pfirst;
			int i=1;
			while (i<=a) 
			{
				while(p->lay[a-i]!=NULL && strcmp(p->lay[a-i]->token,x)<0)
					p=p->lay[a-i];
				p->lay[a-i]=paux->lay[a-i];
				i++;
		        }
			delete [] paux->lay;
			delete paux;
			i=1;
			while(pfirst->lay[pfirst->nl-i]==NULL && i<pfirst->nl)
				i++;
			if(i!=1)
			{
				struct list_elem<T,K,N> **v=new struct 
					list_elem<T,K,N>*[pfirst->nl-i+1];
				memcpy(*v,pfirst->lay,(pfirst->nl-i+1)*
					sizeof(struct list_elem<T,K,N> *));
				pfirst->nl=pfirst->nl-i+1;
				delete [] pfirst->lay;
				*pfirst->lay=*v;
				delete [] *v;
			}			
		}
	}
        
        int isEmpty() 
	{
        	return (pfirst->lay[0]==NULL);
        }

	Skiplist()
	{
		pfirst = new struct list_elem<T,K,N>; 
		pfirst->lay = new struct list_elem<T,K,N>*[1]; 
		pfirst->nl=1;
		pfirst->lay[0]==NULL;
	}
	
	~Skiplist()
	{
	}
};
