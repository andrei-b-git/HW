#include <cstdlib>
#include <ctime>
#include <stdio.h>

//using namespace std;

int main()
{
	srand((unsigned)time(0));
	int random_integer,i;
	for(i=1;i<500;i++)
	{
		random_integer= (rand()%16)+1;
		printf("%d ",random_integer);
	}
	return 0;
}
