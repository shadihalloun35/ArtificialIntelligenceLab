#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>					// for vector class
#include <algorithm>				// for sort algorithm
#include <stack>
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
#include <random>
#include "Hybrid.h"

void Hybrid::GeneticAlgorithm(Matrix* mtx, std::chrono::duration <double> timeAllowed) {
	ga_vector pop_alpha, pop_beta;						//startGeneticAlgorithm.
	ga_vector *population, *buffer;
	int counter = 0;
	int maxColor = mtx->colors - 1;
	struct timeb start, end2;
	int diff = 0;
	ftime(&start);

	init_population(pop_alpha, pop_beta, mtx->dimension, maxColor);
	population = &pop_alpha;
	buffer = &pop_beta;

	for (int i = 0; i < GA_MAXITER; i++) {
		if (diff > maxtime)
			break;
		counter++;
		if (calc_fitness(*population, mtx, maxColor)) {
			//printPop(*population, mtx->dimension);
			mtx->colors = maxColor;
			if (maxColor == 1)
				break;
			maxColor--;
			init_population(*population, *buffer, mtx->dimension, maxColor);
			continue;
		}


		//-----------------------------------------------------------------------------

		sort_by_fitness(*population);			// sort them
		//print_best(*population);				// print the best one
		//print_fitness_info(*population);		// print population fitness average and Standard deviation

		//printPop(*population, mtx->dimension);
		//cout << endl;

		//-----------------------------------------------------------------------------

		switch (SELECTION) {
		case 1:
			mate(*population, *buffer, mtx->dimension, maxColor);				// mate the population together
			break;
		}

		//-----------------------------------------------------------------------------

		swap(population, buffer);		// swap buffers
		ftime(&end2);
		diff = (end2.time - start.time);
	}

	cout << "NUM OF GENERATIONS: " << counter << endl;

}


void Hybrid::GreedyAlgorithm(Matrix* mtx, Node* nodes) {
	for (int i = 0; i < (*mtx).dimension; i++) {
		int color = 1, j = 0;
		while (j < i) {
			if ((*mtx).adj[i][j] == 1 && nodes[j].color == color) {
				color++;
				j = 0;
			}
			else
				j++;
		}
		nodes[i].color = color;
		if (color > (*mtx).colors)
			(*mtx).colors = color;
	}
}

void Hybrid::PrintHybridNodes(Matrix & mtx, Node* nodes) {
	for (int i = 0; i < mtx.dimension; i++) {
		cout << "id: " << nodes[i].id << " ";
		cout << "color: " << nodes[i].color << " ";
		cout << "numOfNeighbors: " << nodes[i].numOfNeighbors << " ";
		cout << endl;
	}
}

void Hybrid::CreateHybridNodes(Node* nodes, int size) {
	for (int i = 0; i < size; i++) {
		nodes[i].id = i;
		nodes[i].color = 0;
		nodes[i].numOfNeighbors = 0;
	}
}

void Hybrid::ActivateHybrid()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	Matrix mtx;
	int* nodes = NULL;
	Node* sorted = NULL;

	srand(unsigned(time(NULL)));
	ifstream input("C:\\ArtificialIntelligenceLab\\ArtificialIntelligenceLab\\CSP\\instances\\myciel3.col");
	if (!input.is_open())
		cout << "Error Opening a file.\n";
	else {
		string x;
		while (getline(input, x)) {
			if (x.size() != 0) {
				if (x[0] == 'c')				//ignore
					continue;
				if (x[0] == 'p') {				//build the adjacency matrix and nodes
					int i = 7;					//to get the dimensions
					while (x[i] != ' ') {
						i++;
					}
					x = x.substr(7, i);
					createMatrix(&mtx, stoi(x));
					nodes = new int[mtx.dimension];
					sorted = new Node[mtx.dimension];
					CreateHybridNodes(sorted, mtx.dimension);
					continue;
				}
				if (x[0] == 'e') {
					int firstNode, secondNode;
					string tmp;
					int i = 2;
					while (x[i] != ' ')
						i++;
					tmp = x.substr(2, i);
					firstNode = stoi(tmp);
					tmp = x.substr(i + 1, x.size());
					secondNode = stoi(tmp);
					mtx.adj[firstNode - 1][secondNode - 1] = 1;
					mtx.adj[secondNode - 1][firstNode - 1] = 1;
					sorted[firstNode - 1].numOfNeighbors++;
					sorted[secondNode - 1].numOfNeighbors++;
				}
			}
		}
		if (mtx.dimension == 0) {
			cout << "Solution: 0 colors";
		}
		GreedyAlgorithm(&mtx, sorted);
		GeneticAlgorithm(&mtx, (std::chrono::duration < double>)15);
		//printMatrix(mtx);
		PrintHybridNodes(mtx, sorted);
		const sec duration = clock::now() - before;
		std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
		getchar();
		getchar();
		input.close();
	}
}
