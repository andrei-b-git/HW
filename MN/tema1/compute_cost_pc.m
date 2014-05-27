% computes a clustering solution total cost
function cost = compute_cost_pc(points, centroids)
	cost = 0;
	% TODO compute clustering solution cost
	[NC, n]=size(centroids);
	[m, n] = size (points);
	AUX = zeros (m, NC);	%o matrice in care retin pt fiecare pct distanta la centroizi
	%construirea matricii AUX
 	for i = 1:NC
      		AUX (:, i) = sqrt(sum (( points - repmat (centroids (i, :), m, 1)).^2, 2));
  	endfor
	%alegerea distantelor minime la fiecare centroid si retinerea lor in "t"
    	[t,c] = min (AUX, [], 2);
	cost = sum (t);
end

