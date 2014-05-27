/*
  nume: Bojin
  prenume: Andrei Florian
  grupa: 313CA
*/

#include<stdio.h>
#include<math.h>

int main(){
	   double x1,x2,y1,y2,d;	// declarea variabilelor, numere reale

	   printf("Pentru punctul A:\n\tx=");	     // citirea coordonatelor punctului A	
	   scanf("%lf",&x1);
	   printf("\ty=");
	   scanf("%lf",&y1);
	   printf("Pentru punctul B:\n\tx=");        // citirea coordonatelor punctului B
	   scanf("%lf",&x2);
	   printf("\ty=");
           scanf("%lf",&y2);

	   d=sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));	// calcularea distantei dintre punctele A si B
	 
           printf("Distanta dintre A si B este %.3lf\n",(double)((long int)(d*1000))/1000);	// afisarea distantei dintre punctele A si B, aliniata la stanga cu 3 zecimale (trunchiata, nu rotunjita)
           return 0;
}
