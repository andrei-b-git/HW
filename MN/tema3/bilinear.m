% nume: Bojin
% prenume: Andrei Florian
% grupa/seria: 313 CA
function bilinear(imagine)
	IMG=imread(imagine);
	[m, n]=size(IMG);
	numefis=["out_bilinear_",substr(imagine,10,length(imagine)-10+1)];
	AUX=zeros(2*m-1,2*n-1);
	AUX(1:2:end,1:2:end)=IMG(:,:);
	AUX(1:2:end,2:2:end)=round((IMG(:,1:(end-1))+IMG(:,2:end))/2);
	AUX1=(IMG(1:(end-1),:)+IMG(2:end,:))/2;
	AUX(2:2:end,1:2:end)=round(AUX1);
	AUX(2:2:end,2:2:end)=round((AUX1(:,1:end-1)+AUX1(:,2:end))/2);
	IMG=AUX';
	out = fopen(numefis,'w');
	fprintf(out, "P2\n"); %%tipul fisierului
	fprintf(out, "%i %i\n", 2*n-1, 2*m-1);
	fprintf(out, "%i\n", 255); %%max = 255 = valoarea_maxima_din_matricea_rezultat
	fprintf(out,"%i\n", IMG);
	fclose(out);
end
