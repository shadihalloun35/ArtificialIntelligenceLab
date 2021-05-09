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
#include "Feasibility.h"

int counter2 = 0;

void Feasibility::PrintFeasibilityNodes(Matrix & mtx, Node* nodes) {
	for (int i = 0; i < mtx.dimension; i++) {
		cout << "id: " << nodes[i].id << " ";
		cout << "color: " << nodes[i].color << " ";
		cout << "numOfNeighbors: " << nodes[i].numOfNeighbors << " ";
		cout << endl;
	}
}

int Feasibility::calcConflict(Matrix & mtx, Node* nodes, int index, int color) {
	int count = 0;
	for (int i = 0; i < mtx.dimension; i++) {
		if (mtx.adj[index][i] == 1) {
			if (nodes[i].color == color)
				count++;
		}
	}
	return count;
}

void Feasibility::tryFix(Matrix & mtx, Node* nodes, int maxColors) {
	int randNode = (rand() % mtx.dimension);									//choose a node randomly
	int tmp = 1;
	int minConflict = mtx.dimension + 1;
	minConflict = calcConflict(mtx, nodes, randNode, tmp);
	for (int i = 2; i <= maxColors; i++) {
		int tmp2;
		tmp2 = calcConflict(mtx, nodes, randNode, i);
		if (tmp2 < minConflict) {
			minConflict = tmp2;
			tmp = i;
		}

	}
	nodes[randNode].color = tmp;
}

void Feasibility::fixGraph(Matrix & mtx, Node* nodes, int color) {
	int removeColor = ((rand() % color) + 1);
	for (int i = 0; i < mtx.dimension; i++) {
		if (nodes[i].color == removeColor)
			nodes[i].color = -1;				//change color randomly
		if (nodes[i].color == color + 1)
			nodes[i].color = removeColor;
	}
	for (int i = 0; i < mtx.dimension; i++) {
		if (nodes[i].color == -1)
			nodes[i].color = ((rand() % color) + 1);				//change color randomly
	}
}

bool Feasibility::isLegal(Matrix mtx, Node* nodes) {
	for (int i = 0; i < mtx.dimension; i++) {
		for (int j = 0; j < mtx.dimension; j++) {
			if (mtx.adj[i][j] == 1) {
				if (nodes[i].color == nodes[j].color)
					return false;
			}
		}
	}
	return true;
}

void Feasibility::greedyAlgorithm(Matrix* mtx, Node* nodes) {
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

void Feasibility::MinimalConflict(Matrix & mtx, Node* nodes, std::chrono::duration<double> timeAllowed) {
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				
	sec duration = clock::now() - before;
	greedyAlgorithm(&mtx, nodes);							//colors the graph greedly
	int maxColors = mtx.colors;
	while (mtx.colors > 1 && duration < timeAllowed) {				//try to reduce number of colors
		counter2++;
		if (isLegal(mtx, nodes)) {
			mtx.colors = maxColors;
			maxColors--;
			fixGraph(mtx, nodes, maxColors);				//change color randomly
		}
		else {
			tryFix(mtx, nodes, maxColors);					//try finding a coloring that makes the graph feasibile
		}
		duration = clock::now() - before;
	}
	mtx.colors = mtx.colors--;
	cout << "SUCCESSFUL" << endl;
	cout << "Number of Colors: " << mtx.colors << endl;
}

void Feasibility::CreateFeasibilityNodes(Node* nodes, int size) {
	for (int i = 0; i < size; i++) {
		nodes[i].id = i;
		nodes[i].color = 0;
		nodes[i].numOfNeighbors = 0;
	}
}

void Feasibility::ActivateFeasibility()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	Matrix mtx;
	int* nodes = NULL;
	Node* sorted = NULL;

	srand(unsigned(time(NULL)));
	counter2 = 0;
	ifstream input("C:\\ArtificialIntelligenceLab\\ArtificialIntelligenceLab\\CSP\\instances\\queen5_5.col");
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
					CreateFeasibilityNodes(sorted, mtx.dimension);
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
		MinimalConflict(mtx, sorted, (std::chrono::duration < double>)15);
		//printMatrix(mtx);
		PrintFeasibilityNodes(mtx, sorted);
		const sec duration = clock::now() - before;
		std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
		cout << "NUM OF STATES: " << counter2 << endl;
		getchar();
		getchar();
		input.close();
	}

}
