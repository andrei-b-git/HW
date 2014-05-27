% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function nearest_neighbor(imagine)
	IMG=imread(imagine);
	[m, n]=size(IMG);
	numefis=["out_nn_",substr(imagine,10,length(imagine)-10+1)];
	AUX=reshape(repmat(IMG',2,1), n, 2*m)';
	IMG=reshape(repmat(AUX',1,2)', length(AUX(:,1)), 2*length(AUX(1,:)));
	IMG=IMG';
	out = fopen(numefis,'w');
	fprintf(out, "P2\n"); %%tipul fisierului
	fprintf(out, "%i %i\n", 2*n, 2*m);
	fprintf(out, "%i\n", 255); %%max = 255 = valoarea_maxima_din_matricea_rezultat
	fprintf(out,"%i\n", IMG);
	fclose(out);
end
