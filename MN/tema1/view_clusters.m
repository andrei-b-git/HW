% outputs a graphical representation of the clustering solution
function view_clusters(points, centroids)
	% TODO graphical representation coded here
	[NC, n]=size(centroids);
	[m, n] = size (points);
	AUX = zeros (m, NC);	%o matrice in care retin pt fiecare pct distanta la centroizi
	Color = zeros(m,3);	%o matrice in care retin pt fiecare pct culoarea corespunzatoare
	%construirea matricii AUX
  	for i = 1:NC
      		AUX (:, i) = sum (( points - repmat (centroids (i, :), m, 1)).^2, 2);
  	endfor
	%alegerea distantelor minime si retinerea in "c" a centroizilor buni
    	[t,c] = min (AUX, [], 2);
	%construirea matricii Color
	for i = 1:m
		Color(i,c(i)) = 1;
	endfor 
	scatter3(points(:, 1), points(:, 2), points(:, 3), [], Color);
end

