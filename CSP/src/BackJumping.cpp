#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm> // for sort algorithm
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
#include "BackJumping.h"


int counter = 0;

bool BackJumping::tryColor(Matrix mtx, BackJumpingNode * nodes, int * org, int index)
{
	for (int i = (nodes[index].color + 1); i <= mtx.colors; i++) { //tries all colors
		int flag = 0;
		for (int j = 0; j < mtx.dimension; j++) { //checks availablity
			if (mtx.adj[nodes[index].id][j] == 1) {
				if (nodes[org[j]].color == i) {
					flag = 1;
					break;
				}
			}
		}
		if (flag)
			continue;
		else {
			nodes[index].color = i;
			return true;
		}
	}
	return false;
}


void updateNeighbors(Matrix mtx, BackJumpingNode* nodes, int* org, int index) {
	for (int i = 0; i < mtx.dimension; i++) {
		if (mtx.adj[index][i] == 1) {
			nodes[org[i]].conflicts.push_back(index);
		}
	}
}

void BackJumping::fixNeighbors(Matrix mtx, BackJumpingNode* nodes, int* org, int index) {
	for (int i = 0; i < mtx.dimension; i++) {
		if (mtx.adj[index][i] == 1) {
			for (size_t j = 0; j < nodes[org[i]].conflicts.size(); j++) {
				if (nodes[org[i]].conflicts[j] == index) {
					nodes[org[i]].conflicts.erase(nodes[org[i]].conflicts.begin() + j);
					break;
				}
			}
		}
	}
}

void BackJumping::resetBackJumpingNodes(Matrix mtx, BackJumpingNode* nodes, int size) {
	for (int i = 0; i < size; i++) {
		nodes[i].color = 0;
		nodes[i].conflicts.clear();
	}
}

void BackJumping::backjump(Matrix mtx, BackJumpingNode* nodes, int* org, int index, int des) {
	int x = index;
	while (x >= 0 && x > des) {
		nodes[x].color = 0;
		fixNeighbors(mtx, nodes, org, nodes[x].id);
		x--;
	}
}

void BackJumping::printBackJumpingNodes(Matrix mtx, BackJumpingNode* nodes) {
	for (int i = 0; i < mtx.dimension; i++) {
		cout << "id: " << nodes[i].id << " ";
		cout << "color: " << nodes[i].color << " ";
		cout << "numOfNeighbors: " << nodes[i].numOfNeighbors << " ";
		cout << "conflicts: ";
		for (size_t j = 0; j < nodes[i].conflicts.size(); j++) {
			cout << nodes[i].conflicts[j] << " ";
		}
		/*cout << "domain: ";
		for (int j = 0; j < nodes[i].domain.size(); j++) {
		cout << nodes[i].domain[j] << " ";
		}*/
		cout << endl;
	}
}


void BackJumping::backtracking(Matrix mtx, BackJumpingNode* nodes, int* org, int index) { //Backtracking with Back jumping
	int Index = index;
	while (1) {
		if (Index >= mtx.dimension) {
			cout << "SUCCESSFUL" << endl;
			cout << "Number of Colors: " << mtx.colors << endl;
			return;
		}
		counter++;
		if (nodes[0].color > 1) {
			resetBackJumpingNodes(mtx, nodes, mtx.dimension); //reset the graph
			mtx.colors++; //increase number of colors available
			Index = 0;
			continue;
		}
		if (!tryColor(mtx, nodes, org, Index)) { //try to color current node in the next color available
			int tmp;
			if (!nodes[Index].conflicts.empty()) {
				tmp = nodes[Index].conflicts.back();
				tmp = org[tmp];
			}
			else {
				tmp = Index - 1;
			}

			backjump(mtx, nodes, org, Index, tmp); //backtrack to the last node that is in the coflict set

			fixNeighbors(mtx, nodes, org, nodes[tmp].id); //fix conflict set and colors of the nodes that has been effected

			Index = tmp;
			if (tmp == 0) {
				resetBackJumpingNodes(mtx, nodes, mtx.dimension);
				mtx.colors++;
				Index = 0;
				continue;
			}
			continue;
		}
		else {
			updateNeighbors(mtx, nodes, org, nodes[Index].id); //update conflict set
			Index++;
			continue;
		}
	}
}

bool BackJumping::sortByNeighbors(BackJumpingNode x, BackJumpingNode y)
{
	return (x.numOfNeighbors > y.numOfNeighbors);
}

void BackJumping::ordering(Matrix mtx, BackJumpingNode* sorted, int* nodes) { //Order neighbors by numOfNeighbors
	sort(sorted, sorted + (mtx.dimension), sortByNeighbors);
	for (int i = 0; i < mtx.dimension; i++) {
		nodes[sorted[i].id] = i;
	}
}

void BackJumping::createBackJumpingNodes(BackJumpingNode* nodes, int size) {
	for (int i = 0; i < size; i++) {
		nodes[i].id = i;
		nodes[i].color = 0;
		nodes[i].numOfNeighbors = 0;
	}
}

void BackJumping::ActivateBackJumping()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time

	Matrix mtx;
	int* nodes = NULL;
	BackJumpingNode* sorted = NULL;
	ifstream input("C:\\ArtificialIntelligenceLab\\ArtificialIntelligenceLab\\CSP\\instances\\myciel3.col");
	if (!input.is_open())
	{
		cout << "Error Opening a file.\n";
		getchar();
		getchar();
	}
	else
	{
		string x;
		while (getline(input, x)) {
			if (x.size() != 0) {
				if (x[0] == 'c') //ignore
					continue;
				if (x[0] == 'p') { //build the adjacency matrix and nodes
					int i = 7; //to get the dimensions
					while (x[i] != ' ') {
						i++;
					}
					x = x.substr(7, i);
					createMatrix(&mtx, stoi(x));
					nodes = new int[mtx.dimension];
					sorted = new BackJumpingNode[mtx.dimension];
					createBackJumpingNodes(sorted, mtx.dimension);
					continue;
				}
				if (x[0] == 'e') {
					int firstBackJumpingNode, secondBackJumpingNode;
					string tmp;
					int i = 2;
					while (x[i] != ' ')
						i++;
					tmp = x.substr(2, i);
					firstBackJumpingNode = stoi(tmp);
					tmp = x.substr(i + 1, x.size());
					secondBackJumpingNode = stoi(tmp);
					mtx.adj[firstBackJumpingNode - 1][secondBackJumpingNode - 1] = 1;
					mtx.adj[secondBackJumpingNode - 1][firstBackJumpingNode - 1] = 1;
					sorted[firstBackJumpingNode - 1].numOfNeighbors++;
					sorted[secondBackJumpingNode - 1].numOfNeighbors++;
				}
			}
		}
		if (mtx.dimension == 0)
		{
			cout << "Solution: 0 colors";
			getchar();
			getchar();
		}
		else {
			ordering(mtx, sorted, nodes); //highest num of neighbors are first

			backtracking(mtx, sorted, nodes, 0);
		}
		const sec duration = clock::now() - before;
		std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
		cout << "NUM OF STATES: " << counter << endl;

		//printMatrix(mtx);
		printBackJumpingNodes(mtx, sorted);
		getchar();
		getchar();
		input.close();
	}

}

