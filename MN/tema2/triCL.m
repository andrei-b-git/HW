% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function x = triCL(A,b)
	[nzval, colind, rowptr] = clFormat(A); %descompunerea matricii A;
	[n, n1] = size (A); %retinerea in n a dimensiunii matricii patratice A;
	%algoritmul pt rezolvarea Ax=b pe linii, modificat pt cei trei vectori;
	x = b;
	for i = 1:n
		for j = rowptr (i):(rowptr (i+1)-2)
			x (i) = x (i) - nzval (j) * x (colind (j));
		endfor
		x (i) = x (i) / nzval (rowptr (i+1)-1);
	endfor
end
