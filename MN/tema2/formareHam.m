% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function cod = formareHam(sir)
	[n1, n] = size (sir); %retinerea in n a dimensiunii codului initial;
	i=2; %numarul de biti de paritate, minim 2;
	%calcularea lui i ca fiind prima putere a lui 2 care sa fie mai mare
	%decat lungimea codului Hamming (n+i);
	while (n+i >= 2^i)
		i++;
	endwhile
	m = n+i; %lungimea codului Hamming;
	cod = zeros (1, m); %codul Hamming, initial un vector de 0;
	k = 0;
	%transferul codului initial in codul Hamming, pe pozitiile diferite de
	%cele ale bitilor de paritate;
	for i = 1:m
		if (log2 (i) - floor (log2 (i)) != 0)
			k++;
			cod (i) = sir (k) - '0';
		endif
	endfor
	%parcurgerea bitilor de paritate si calcularea acestora;
	for i = 0:m-n-1
		s=0; %suma bitilor corespunzatori fiecarui bit de paritate;
		j=2^i; %pozitia bitului de paritate curent;
		while (j<=m)
			for k = 1:2^i %parcurgerea a atator biti cat este bitul
			%de paritate;
				if (j+k-1 > m)
					break;
				else
					s+=cod(j+k-1); %calculul sumei;
				endif
			endfor
			j=j+k+2^i; %sarirea peste atatia biti cat este bitul de 
			%paritate;
		endwhile	
		cod(2^i) = mod(s,2); %bitul de paritate este 0 sau 1, in functie
		%de paritatea sumei celorlalti biti corespunzatori;
	endfor
	%transformarea codului Hamming din vector in string cu eliminarea 
	%spatiilor goale;
	cod = strrep(num2str(cod)," ",""); 
end
