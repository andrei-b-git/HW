% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function [nzval, colind, rowptr] = clFormat(A)
	[n, n1] = size (A); %retinerea in n a dimensiunii matricii patratice A;
	nz = 0; %numarul elementelor nenule, initial 0;
	rowptr = ones (1, n+1); %construirea ca un vector de 1 cu dim n+1;
	%parcurgerea matricii A si numararea elementelor nenule, retinerea 
	%acestora in nzval si a indicelui coloanei pe care se afla fiecare in
	%colind;
	for i = 1:n
		for j = 1:n
			if (A (i, j) != 0)
				nz++;
				nzval (nz) = A (i, j);
				colind (nz) = j;
			endif
		endfor
		if (nz+1 == rowptr(i)) %daca nr elem nenule nu s-a schimbat
		%fata de cel de pe linia anterioara;
			rowptr (i) = -1; %marcarea cu -1 a liniei nule;
		endif
		rowptr (i+1) = nz+1; %formula pt construirea lui rowptr;
	endfor 
end
