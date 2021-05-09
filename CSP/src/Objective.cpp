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
#include "Objective.h"
#define STARTING_TEMP 1000
#define ENDING_TEMP 0.01f
#define COOLING_RATE 0.99f

int counter3 = 0;

void Objective::Retreat(Node* nodes, int* oldColors, int size) {
	for (int i = 0; i < size; i++) {
		nodes[i].color = oldColors[i];
	}
}

void Objective::SwapCand(vector<int>* cand1, vector<int>* cand2) {
	(*cand1).clear();
	int size = (*cand2).size();
	for (int i = 0; i < size; i++) {
		(*cand1).push_back((*cand2)[i]);
	}
	(*cand2).clear();
}

bool Objective::ContainsInCand(vector<int>* cand1, int index) {
	int size = (*cand1).size();
	for (int i = 0; i < size; i++) {
		if ((*cand1)[i] == index)
			return true;
	}
	return false;
}

void Objective::PushConflicted(Matrix* mtx, Node* nodes, vector<int>* cand1, vector<int>* cand2, int color) {
	int size = (*cand2).size();
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < (*mtx).dimension; j++) {
			if ((*mtx).adj[(*cand2)[i]][j] == 1 && nodes[j].color == color) {
				if (!ContainsInCand(cand1, j))
					(*cand1).push_back(j);
			}
		}
	}
	(*cand2).clear();
}

void Objective::ChangeColor(vector<int>* cand1, Node* nodes, int color) {
	int size = (*cand1).size();
	for (int i = 0; i < size; i++) {
		nodes[(*cand1)[i]].color = color;
	}
}

int Objective::KempeChain(Matrix* mtx, Node* nodes) {
	int randNode = (rand() % (*mtx).dimension);
	int color1 = nodes[randNode].color;
	int color2 = ((rand() % (*mtx).colors) + 1);
	vector<int> cand1;
	vector<int> cand2;
	cand1.clear();
	cand2.clear();
	cand1.push_back(randNode);
	while (!cand1.empty()) {
		ChangeColor(&cand1, nodes, color2);
		PushConflicted(mtx, nodes, &cand2, &cand1, color2);
		int tmp = color1;
		color1 = color2;
		color2 = tmp;
		SwapCand(&cand1, &cand2);

	}
	return CalcCi(mtx, nodes, mtx->dimension, mtx->colors);
}

void Objective::CopyColors(Node* nodes, int* oldColors, int size) {
	for (int i = 0; i < size; i++) {
		oldColors[i] = nodes[i].color;
	}
}

void Objective::FixColorsInGraph(int size, Node* nodes, int colorDeleted, int maxColors) {
	for (int i = 0; i < size; i++) {
		if (nodes[i].color == maxColors)
			nodes[i].color = colorDeleted;
	}
}

int Objective::CalcCi(Matrix* mtx, Node* nodes, int size, int colors) {
	int* cols = new int[colors + 1];
	int sum = 0;
	for (int i = 0; i < colors + 1; i++) {
		cols[i] = 0;
	}
	for (int i = 0; i < size; i++) {
		cols[nodes[i].color]++;
	}
	for (int i = 1; i < colors + 1; i++) {
		if (cols[i] == 0) {										//we moved from k to k-1
			mtx->colors--;
			FixColorsInGraph(mtx->dimension, nodes, i, mtx->colors + 1);
		}
		sum += pow(cols[i], 2);
	}
	return sum;
}

int Objective::GreedyAlgorithm(Matrix* mtx, Node* nodes) {
	int Ci = 0;
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
	return CalcCi(mtx, nodes, mtx->dimension, mtx->colors);
}

void Objective::SimulatedAnnealing(Matrix mtx, Node* nodes, std::chrono::duration < double> timeAllowed) {

	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();
	sec duration = clock::now() - before;

	int startTemp = STARTING_TEMP;
	double endTemp = double(ENDING_TEMP);
	double temp = double(startTemp);
	int Ci = GreedyAlgorithm(&mtx, nodes);
	while (mtx.colors > 1 && duration < timeAllowed) {
		counter3++;
		int* oldColors = new int[mtx.dimension];
		CopyColors(nodes, oldColors, mtx.dimension);
		int tmp = KempeChain(&mtx, nodes);
		if (tmp > Ci) {
			Ci = tmp;
		}
		else {
			if ((rand() % startTemp) > temp) {
				Retreat(nodes, oldColors, mtx.dimension);
			}
			else 
			{
				Ci = tmp;
			}
		}
		temp *= COOLING_RATE;
		duration = clock::now() - before;
		
	}
	cout << "SUCCESSFUL" << endl;
	cout << "Number of Colors: " << mtx.colors << endl;
}

void Objective::PrintObjectiveNodes(Matrix & mtx, Node * nodes)
{
	for (int i = 0; i < mtx.dimension; i++) {
		cout << "id: " << nodes[i].id << " ";
		cout << "color: " << nodes[i].color << " ";
		cout << "numOfNeighbors: " << nodes[i].numOfNeighbors << " ";
		cout << endl;
	}
}


void Objective::CreateObjectiveNodes(Node* nodes, int size) {
	for (int i = 0; i < size; i++) {
		nodes[i].id = i;
		nodes[i].color = 0;
		nodes[i].numOfNeighbors = 0;
	}
}

void Objective::ActivateObjective()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	Matrix mtx;
	int* nodes = NULL;
	Node* sorted = NULL;

	srand(unsigned(time(NULL)));
	counter3 = 0;
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
					CreateObjectiveNodes(sorted, mtx.dimension);
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
		SimulatedAnnealing(mtx, sorted, (std::chrono::duration < double>)15);
		//printMatrix(mtx);
		PrintObjectiveNodes(mtx, sorted);
		const sec duration = clock::now() - before;
		std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
		cout << "NUM OF STATES: " << counter3 << endl;
		getchar();
		getchar();
		input.close();
	}

}