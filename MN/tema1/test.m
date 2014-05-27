%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                    Tema 1 Metode Numerice, 2012                          %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  NU MODIFICATI acest fisier, el va fi inlocuit la testare cu continutul  %
% initial. Fiecare functie trebuie implementata in fisierul corespunzator. %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% test entire homework
function score = test(indexes)
	more off
	
	% set scores per task
	score_clustering = 3;
	score_cost = 2;

	% all indexes?
	all_indexes = [1 : 10];
	if exist("indexes", "var") == 0
		indexes = all_indexes;
	end
	
	% set score vectors
	test_count = length(indexes);
	pass_clustering = zeros(test_count, 1);
	pass_cost = zeros(test_count, 1);

	% compute total score
	total_score = test_count * (score_cost + score_clustering);

	% test
	for k = [1 : test_count]
		printf("Testing %02d...\n", indexes(k));
		printf("\tClustering   ");
		pass_clustering(k) = test_clustering_by_index(indexes(k));
		if pass_clustering(k) == 1
			printf("[ OK ]");
		else
			printf("[FAIL]");
		end
		% compute student score and total score
		score = sum(pass_clustering) * score_clustering + sum(pass_cost) * score_cost;
		printf("\tCurrent score: %02d / %02d\n", score, total_score); 
		
		printf("\tCost         ");
		pass_cost(k) = test_cost_by_index(indexes(k));
		if pass_cost(k) == 1
			printf("[ OK ]");
		else
			printf("[FAIL]");
		end
		% compute student score and total score
		score = sum(pass_clustering) * score_clustering + sum(pass_cost) * score_cost;
		printf("\tCurrent score: %02d / %02d\n", score, total_score); 
	end
	
	
	% write summary
	printf("\n");
	if total_score > score
		printf("You didn't pass all tests. :-(\n");
		printf("Check the above listing for details.\n");
	else
		printf("All tests passed! :-)\n");
	end

	% write score
	printf("\nScore: %d/%d\n", score, total_score);
end

test

