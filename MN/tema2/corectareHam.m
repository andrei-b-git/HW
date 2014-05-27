% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function cod = corectareHam(sir)
	[n1, m] = size (sir); %retinerea in m a dimensiunii codului Hamming;
	i=2; %numarul de biti de paritate, minim 2;
	%calcularea lui i ca fiind prima putere a lui 2 care sa fie mai mare
	%decat lungimea codului Hamming (m);
	while (m >= 2^i)
		i++;
	endwhile
	n = m-i; %lungimea codului initial;
	cod = zeros (1, m); %codul Hamming nou, initial un vector de 0;
	%transferul codului Hamming din string intr-un vector;
	for i = 1:m
			cod (i) = sir (i) - '0';
	endfor
	bit=0; %pozitia bitului care este gresit;
	%parcurgerea bitilor de paritate si recalcularea acestora;
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
		if (cod(2^i) != mod(s-cod(2^i),2)) %daca bitul de paritate curent
		%este diferit fata de cel recalculat;
			bit+=2^i; %adunarea bitului de paritate gresit la 
			%pozitia bitului care trebuie corectat;
		endif
	endfor
	if (bit!=0) %daca exista un bit gresit;
		cod(bit)=mod(cod(bit)+1,2); %corectarea bitului gresit;
	endif
	%transformarea codului Hamming din vector in string cu eliminarea 
	%spatiilor goale;
	cod = strrep(num2str(cod)," ","");
end
