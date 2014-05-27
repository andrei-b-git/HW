% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function x = triCC(A,b)
	[nzval, rowind, colptr] = ccFormat(A); %descompunerea matricii A;
	[n, n1] = size (A); %retinerea in n a dimensiunii matricii patratice A;
	%algoritmul pt rezolvarea Ax=b pe coloane, modificat pt cei trei vectori;
	x = b;
	for j = 1:n
		x (j) = x (j) / nzval (colptr (j));
		for i = (colptr (j)+1):(colptr (j+1)-1)
			x (rowind (i)) = x (rowind (i)) - nzval(i) * x (j);
		endfor
	endfor
end
