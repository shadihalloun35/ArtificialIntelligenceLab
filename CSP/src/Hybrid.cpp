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

#define GA_POPSIZE		4000		// ga population size
#define GA_MAXITER		1000		// maximum iterations
#define GA_ELITRATE		0.1f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define	K				5			//Tournament size 
#define	MAX_AGE			10			//Max Age
#define OM	            0.05f		//OM rate
#define OM_RATE         RAND_MAX * OM
#define MAX_OM          10
#define SELECTION		1
#define CROSSOVER		1


float avg = 0;
float dev = 0;

bool fitness_sort(ga_struct x, ga_struct y)
{
	return (x.fitness < y.fitness);
}

inline void sort_by_fitness(ga_vector &population)
{
	sort(population.begin(), population.end(), fitness_sort);
}

int Hybrid::calcBj(ga_struct &member, Matrix* mtx, int color) {
	int count = 0;
	for (int i = 0; i < mtx->dimension - 1; i++) {
		if (member.colors[i] == color) {
			for (int j = i + 1; j < mtx->dimension; j++) {
				if (mtx->adj[i][j] == 1 && member.colors[j] == color)
					count++;
			}
		}
	}
	return count;
}

int Hybrid::calcCj(ga_struct &member, Matrix* mtx, int color) {
	int count = 0;
	for (int i = 0; i < mtx->dimension; i++)
		if (member.colors[i] == color)
			count++;
	return count;
}

int Hybrid::calc_fitness(ga_vector &population, Matrix* mtx, int maxColors)
{
	int flag = 1;
	int flag2 = 0;
	int tsize = mtx->dimension;
	int fitness;
	avg = 0;
	dev = 0;

	for (int i = 0; i < GA_POPSIZE; i++) {
		flag = 1;
		fitness = 0;
		int CiS = 0;
		for (int j = 1; j <= maxColors; j++) {
			int Bj = calcBj(population[i], mtx, j);
			if (Bj != 0)
				flag = 0;
			int Cj = calcCj(population[i], mtx, j);
			CiS += pow(Cj, 2);
			fitness += (2 * Bj*Cj);
		}
		fitness = fitness - CiS;
		population[i].fitness = fitness;
		avg += fitness;
		if (flag == 1) {
			flag2 = 1;
		}

	}
	avg /= GA_POPSIZE;

	for (int i = 0; i < GA_POPSIZE; i++) {
		dev += (pow(population[i].fitness - avg, 2));
	}
	dev /= GA_POPSIZE;
	dev = sqrt(dev);
	return flag2;
}

void Hybrid::init_population(ga_vector &population, ga_vector &buffer, int tsize, int maxColor)
{
	buffer.clear();
	population.clear();
	buffer.resize(GA_POPSIZE);
	for (int i = 0; i < GA_POPSIZE; i++) {
		buffer[i].colors = new int[tsize];
		ga_struct citizen;						//a graph
		citizen.colors = new int[tsize];
		citizen.fitness = 0;
		citizen.age = 0;			//reset age

		for (int j = 0; j < tsize; j++)
			citizen.colors[j] = (rand() % maxColor) + 1;

		population.push_back(citizen);
	}

}

void elitism(ga_vector &population, ga_vector &buffer, int esize, int tsize)
{
	for (int i = 0; i < esize; i++) {
		//buffer[i].colors = population[i].colors;
		for (int j = 0; j < tsize; j++) {
			buffer[i].colors[j] = population[i].colors[j];
		}
		buffer[i].fitness = population[i].fitness;
		buffer[i].age++;
	}
}

void Hybrid::orderMutate(ga_struct &member, int tsize)
{
	int num = ((rand() % MAX_OM) + 2);
	vector<int> cand1;
	vector<int> cand2;
	for (int i = 0; i < tsize; i++) {
		if (cand1.size() >= num)
			break;
		if (rand() < OM_RATE) {
			cand1.push_back(member.colors[i]);
			cand2.push_back(i);
		}
	}
	random_shuffle(cand1.begin(), cand1.end());
	while (!cand2.empty()) {
		member.colors[cand2.size() - 1] = cand1[cand1.size() - 1];
		cand2.pop_back();
		cand1.pop_back();
	}
}

void Hybrid::crossoverNmutate(ga_vector &population, ga_vector &buffer, int* pointers, int tsize) {
	int esize = GA_POPSIZE * GA_ELITRATE;

	string tmp;

	for (int i = esize; i < GA_POPSIZE; i++) {
		int spos1 = rand() % tsize;
		int spos2 = rand() % tsize;
		if (spos1 > spos2) {
			int tmp;
			tmp = spos1;
			spos1 = spos2;
			spos2 = tmp;
		}

		switch (CROSSOVER) {
		case 1:												//one point cross-over
			for (int j = 0; j < tsize; j++) {
				if (j < spos1)
					buffer[i].colors[j] = population[pointers[2 * (i - esize)]].colors[j];
				else
					buffer[i].colors[j] = population[pointers[2 * (i - esize) + 1]].colors[j];
			}
			break;
		case 2:												// two point cross-over
			for (int j = 0; j < tsize; j++) {
				if (j < spos1)
					buffer[i].colors[j] = population[pointers[2 * (i - esize)]].colors[j];
				else {
					if (j < spos2)
						buffer[i].colors[j] = population[pointers[2 * (i - esize) + 1]].colors[j];
					else
						buffer[i].colors[j] = population[pointers[2 * (i - esize)]].colors[j];
				}
			}
			break;
		case 3:												// uniform cross-over
			for (int j = 0; j < tsize; j++) {

				spos1 = rand() % 2;
				if (spos1 == 0)
					buffer[i].colors[j] = population[pointers[2 * (i - esize)]].colors[j];
				else
					buffer[i].colors[j] = population[pointers[2 * (i - esize) + 1]].colors[j];
			}
			break;
		}

		if (rand() < GA_MUTATION) {
			orderMutate(buffer[i], tsize);								//mutate one letter only
		}
		buffer[i].age = 0;
	}
}

void Hybrid::mate(ga_vector &population, ga_vector &buffer, int tsize, int maxColor)
{
	int esize = GA_POPSIZE * GA_ELITRATE;
	int parents = 2 * (GA_POPSIZE - esize);
	int i1;
	int* pointers = new int[parents];

	elitism(population, buffer, esize, tsize);		//insert best 0.1 citizens of the population to the buffer

	// Mate the rest
	for (int i = 0; i < parents; i++) {
		i1 = rand() % (GA_POPSIZE / 2);
		pointers[i] = i1;
	}
	crossoverNmutate(population, buffer, pointers, tsize);
}

void Hybrid::GeneticAlgorithm(Matrix* mtx, std::chrono::duration <double> timeAllowed) {
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();
	sec duration = clock::now() - before;

	ga_vector pop_alpha, pop_beta;						//startGeneticAlgorithm.
	ga_vector *population, *buffer;
	int counter = 0; 
	int maxColor = mtx->colors - 1;

	init_population(pop_alpha, pop_beta, mtx->dimension, maxColor);
	population = &pop_alpha;
	buffer = &pop_beta;

	for (int i = 0; i < GA_MAXITER; i++) {
		if (duration > timeAllowed)
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
		duration = clock::now() - before;

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
	}

	cout << "Number of Colors: " << mtx->colors << endl;

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

void Hybrid::ActivateHybrid(string filePath, int timeAllowed)
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	Matrix mtx;
	int* nodes = NULL;
	Node* sorted = NULL;

	srand(unsigned(time(NULL)));
	ifstream input(filePath);
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
					string line = x;
					x = x.substr(7, i);
					cout << "Input Features:" << std::endl;
					cout << "Number of Nodes: " << stoi(x) << std::endl;
					cout << "Number of Edges: " << line.substr(i, line.size() - 1) << std::endl;
					int nodesNum = stoi(x);
					int edgesNum = stoi(line.substr(i, line.size() - 1));
					cout << "Density of the Graph: " << (float)2 * edgesNum / (nodesNum*(nodesNum - 1)) << std::endl;
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
		GeneticAlgorithm(&mtx, (std::chrono::duration < double>)timeAllowed);
		//printMatrix(mtx);
		PrintHybridNodes(mtx, sorted);
		const sec duration = clock::now() - before;
		std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
		getchar();
		getchar();
		input.close();
	}
}
