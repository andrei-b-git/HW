% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function [nzval, rowind, colptr] = ccFormat(A)
	[n, n1] = size (A); %retinerea in n a dimensiunii matricii patratice A;
	nz = 0; %numarul elementelor nenule, initial 0;
	colptr = ones (1, n+1); %construirea ca un vector de 1 cu dim n+1;
	%parcurgerea matricii A si numararea elementelor nenule, retinerea 
	%acestora in nzval si a indicelui liniei pe care se afla fiecare in
	%rowind;
	for j = 1:n
		for i = 1:n
			if (A (i, j) != 0)
				nz++;
				nzval (nz) = A (i, j);
				rowind (nz) = i;
			endif
		endfor
		if (nz+1 == colptr(j)) %daca nr elem nenule nu s-a schimbat
		%fata de cel de pe coloana anterioara;
			colptr (j) = -1; %marcarea cu -1 a coloanei nule;
		endif
		colptr (j+1) = nz+1; %formula pt construirea lui colptr;
	endfor 
end
