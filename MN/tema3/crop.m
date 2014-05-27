% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function crop(imagine)
	IMG=imread(imagine);
	[m, n]=size(IMG);
	numefis=["out_crop_",imagine];
	IMG=IMG(round(m/4+1):round(3*m/4),round(n/4+1):round(3*n/4));
	IMG=IMG';
	out = fopen(numefis,'w');
	fprintf(out, "P2\n"); %%tipul fisierului
	fprintf(out, "%i %i\n", n/2, m/2);
	fprintf(out, "%i\n", 255); %%max = 255 = valoarea_maxima_din_matricea_rezultat
	fprintf(out,"%i\n", IMG);
	fclose(out);
end
