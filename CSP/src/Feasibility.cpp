#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>					// for vector class
#include <algorithm>				// for sort algorithm
#include <stack>
#include <time.h>					// for random seed
#include <sys/timeb.h>
#include <random>
#include "Feasibility.h"

/**
int main(int argc, char *argv[])
{
	Matrix mtx;
	int* nodes = NULL;
	Node* sorted = NULL;
	int max = argc;



	srand(unsigned(time(NULL)));
	if (max < 2) {
		cout << "You must add an Input";
		return 0;
	}

	for (int i = 2; i < max; i++) {
		struct timeb start, end2;
		int diff;
		ftime(&start);
		counter = 0;
		ifstream input(argv[i]);
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
						createNodes(sorted, mtx.dimension);
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
			minimalConflict(mtx, sorted, atoi(argv[1]));
			//printMatrix(mtx);
			//printNodes(mtx, sorted);
			ftime(&end2);
			diff = (int)(1000.0 * (end2.time - start.time)
				+ (end2.millitm - start.millitm));
			cout << "Elapsed Time: " << diff << endl;
			cout << "NUM OF STATES: " << counter << endl;
			input.close();
		}
	}
	return 0;
}
*/