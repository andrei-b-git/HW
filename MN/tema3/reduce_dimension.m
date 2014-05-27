% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function reduce_dimension (imagine, rate)
	IMG=imread(imagine);
	[m, n]=size(IMG);
	numefis=["out_rd_",num2str(rate),"_",imagine];
	IMG=IMG-round(rate/100*IMG);
	IMG=IMG';
	out = fopen(numefis,'w');
	fprintf(out, "P2\n"); %%tipul fisierului
	fprintf(out, "%i %i\n", n, m);
	fprintf(out, "%i\n", 255); %%max = 255 = valoarea_maxima_din_matricea_rezultat
	fprintf(out,"%i\n", IMG);
	fclose(out);
end
