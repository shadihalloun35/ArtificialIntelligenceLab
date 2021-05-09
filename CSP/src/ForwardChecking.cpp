#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <algorithm> // for sort algorithm
#include <stack>
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
#include "ForwardChecking.h"

//FW AND AC FINAL

int counter1 = 0;

void ForwardChecking::printDeletedForwardCheckingNodes(vector<Deleted> x) {
	for (size_t i = 0; i < x.size(); i++)
		cout << "ForwardCheckingNode 1: " << x[i].node1.id << " " << "ForwardCheckingNode 2: " << x[i].node2.id << " " << "Color: " << x[i].color << endl;
	cout << endl;
}


bool ForwardChecking::checkAvailabilty(ForwardCheckingNode* nodes, int index, int color) {
	for (size_t i = 0; i < nodes[index].domain.size(); i++) {
		if (nodes[index].domain[i] == color)
			return true;
	}
	return false;
}

void ForwardChecking::deleteColorFromDomain(ForwardCheckingNode* nodes, int index, int color) {
	for (size_t i = 0; i < nodes[index].domain.size(); i++) {
		if (nodes[index].domain[i] == color) {
			nodes[index].domain.erase(nodes[index].domain.begin() + i);
			break;
		}
	}
}

void ForwardChecking::fixNeighbors(Matrix mtx, ForwardCheckingNode* nodes, int index, vector<Deleted>* deleted) {
	size_t i = 0;
	while (i < (*deleted).size()) {
		if ((*deleted)[i].node1.id == index) {
			nodes[((*deleted)[i]).node2.id].domain.push_back(((*deleted)[i]).color);
			(*deleted).erase((*deleted).begin() + i);
		}
		else
			i++;
	}
}

void ForwardChecking::fixConsistency(ForwardCheckingNode* nodes, int index1, int index2, int color, vector<Deleted>* deleted) {
	Deleted tmp;
	tmp.node1 = nodes[index1];
	tmp.node2 = nodes[index2];
	tmp.color = color;
	(*deleted).push_back(tmp);
	deleteColorFromDomain(nodes, index2, color);
}

int ForwardChecking::checkConsistency(Matrix mtx, ForwardCheckingNode* nodes, int source, int index, vector<Deleted>* deleted) {
	int flag2 = 0;
	for (int i = 0; i < mtx.dimension; i++) {
		if (mtx.adj[index][i] == 1 && nodes[i].color == 0) {
			for (size_t j = 0; j < nodes[index].domain.size(); j++) {
				int flag = 0;
				for (size_t k = 0; k < nodes[i].domain.size(); k++) {
					if (nodes[index].domain[j] != nodes[i].domain[k]) {
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					flag2 = 1;
					fixConsistency(nodes, source, index, nodes[index].domain[j], deleted);
					if (nodes[index].domain.empty()) {
						fixNeighbors(mtx, nodes, source, deleted);
						return 2;
					}
				}
			}
		}
	}
	if (flag2 == 1)
		return 1;
	return 0;
}

bool ForwardChecking::arcConsistency(Matrix mtx, ForwardCheckingNode* nodes, int source, vector<Deleted>* deleted) {
	int consistent = 1; // 0 -> consistent
	while (consistent == 1) {
		for (int i = 0; i < mtx.dimension; i++) {
			consistent = checkConsistency(mtx, nodes, source, i, deleted);
			if (consistent == 0)
				return true;
			if (consistent == 2)
				return false;
		}
	}
	return true;
}

void ForwardChecking::forwardChecking(Matrix mtx, ForwardCheckingNode* nodes, int index, int color, vector<Deleted>* deleted) {
	for (int i = 0; i < mtx.dimension; i++) {
		if (mtx.adj[index][i] == 1 && nodes[i].color == 0) {
			Deleted tmp;
			tmp.node1 = nodes[index];
			tmp.node2 = nodes[i];
			tmp.color = color;
			(*deleted).push_back(tmp);
			deleteColorFromDomain(nodes, i, color);
		}
	}
}

bool ForwardChecking::tryColor(Matrix mtx, ForwardCheckingNode* nodes, int index, vector<Deleted>* deleted) {
	for (int i = (nodes[index].color + 1); i <= mtx.colors; i++) { //tries all colors
		if (!checkAvailabilty(nodes, index, i))
			continue;
		nodes[index].color = i;
		size_t j = 0;
		while (j < nodes[index].domain.size()) {
			if (nodes[index].domain[j] != i) {
				Deleted tmp;
				tmp.node1 = nodes[index];
				tmp.node2 = nodes[index];
				tmp.color = i;
				(*deleted).push_back(tmp);
				nodes[index].domain.erase(nodes[index].domain.begin() + j);
			}
			else
				j++;
		}
		forwardChecking(mtx, nodes, index, i, deleted);
		if (arcConsistency(mtx, nodes, index, deleted)) {
			// printDeletedForwardCheckingNodes(*deleted);
			return true;
		}
	}
	return false;
}

void ForwardChecking::resetForwardCheckingNodes(Matrix mtx, ForwardCheckingNode* nodes, int size) {
	for (int i = 0; i < size; i++) {
		nodes[i].color = 0;
		nodes[i].domain.clear();
		for (int j = 1; j <= mtx.colors; j++) {
			nodes[i].domain.push_back(j);
		}
	}
}

void ForwardChecking::printForwardCheckingNodes(Matrix mtx, ForwardCheckingNode* nodes) {
	for (int i = 0; i < mtx.dimension; i++) {
		cout << "id: " << nodes[i].id << " ";
		cout << "color: " << nodes[i].color << " ";
		cout << "numOfNeighbors: " << nodes[i].numOfNeighbors << " ";
		cout << "domain: ";
		for (size_t j = 0; j < nodes[i].domain.size(); j++) {
			cout << nodes[i].domain[j] << " ";
		}
		cout << endl;
	}
}

int ForwardChecking::nextForwardCheckingNode(Matrix mtx, ForwardCheckingNode* nodes, int index) {
	int cand = -1;
	size_t val = mtx.dimension + 1;
	for (int i = 0; i < mtx.dimension; i++) { //MRV
		if (mtx.adj[index][i] == 1 && nodes[i].color == 0) {
			if (nodes[i].domain.size() < val) {
				val = nodes[i].domain.size();
				cand = i;
			}
		}
	}
	if (cand == -1) { // all neighbors are colored
		for (int i = 0; i < mtx.dimension; i++) {
			if (nodes[i].color == 0)
				return i;
		}
		return -1;
	}
	else {
		return cand;
	}
}

void ForwardChecking::FCandAC(Matrix mtx, ForwardCheckingNode* nodes, int index) { //Forward Checking & Arc Consistency
	stack<int> coloredForwardCheckingNodes;
	vector<Deleted> deleted;
	int Index = index;
	while (1)
	{
		counter1++;
		if (Index == 0 && nodes[0].color != 0)
		{
			mtx.colors++;
			resetForwardCheckingNodes(mtx, nodes, mtx.dimension);
			while (!coloredForwardCheckingNodes.empty())
				coloredForwardCheckingNodes.pop();
			deleted.clear();
			continue;
		}
		fixNeighbors(mtx, nodes, Index, &deleted);
		if (!tryColor(mtx, nodes, Index, &deleted))
		{
			if (Index == 0) {
				mtx.colors++;
				resetForwardCheckingNodes(mtx, nodes, mtx.dimension);
				while (!coloredForwardCheckingNodes.empty())
					coloredForwardCheckingNodes.pop();
				deleted.clear();
				continue;
			}
			int tmp = coloredForwardCheckingNodes.top();
			coloredForwardCheckingNodes.pop();
			nodes[Index].color = 0;
			Index = tmp;
			continue;
		}
		else
		{ //succeeded coloring
			coloredForwardCheckingNodes.push(Index);
			Index = nextForwardCheckingNode(mtx, nodes, Index);
			if (Index == -1)
			{
				printForwardCheckingNodes(mtx, nodes);
				cout << endl;
				cout << "SUCCESSFUL" << endl;
				cout << "Number of Colors: " << mtx.colors << endl;
				return;
			}
			continue;
		}
	}
}

bool ForwardChecking::sortByNeighbors(ForwardCheckingNode x, ForwardCheckingNode y)
{
	return (x.numOfNeighbors > y.numOfNeighbors);
}

void ForwardChecking::ordering(Matrix mtx, ForwardCheckingNode* sorted, int* nodes) {
	sort(sorted, sorted + (mtx.dimension), sortByNeighbors);
	for (int i = 0; i < mtx.dimension; i++) {
		nodes[sorted[i].id] = i;
	}
}

void ForwardChecking::createForwardCheckingNodes(ForwardCheckingNode* nodes, int size) {
	for (int i = 0; i < size; i++) {
		nodes[i].id = i;
		nodes[i].color = 0;
		nodes[i].numOfNeighbors = 0;
		nodes[i].domain.clear();
		nodes[i].domain.push_back(1);
	}
}

void ForwardChecking::ActivateForwardChecking()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	Matrix mtx;
	int* nodes = NULL;
	ForwardCheckingNode* sorted = NULL;

	//cout << "You must add an Input";


	ifstream input("C:\\ArtificialIntelligenceLab\\ArtificialIntelligenceLab\\CSP\\instances\\myciel3.col");
	if (!input.is_open())
	{
		cout << "Error Opening a file.\n";
		getchar();
		getchar();
	}
	else {
		string x;
		while (getline(input, x)) {
			if (x.size() != 0) {
				if (x[0] == 'c') //ignore - we don't need
					continue;
				if (x[0] == 'p') { //build the adjacency matrix and nodes
					int i = 7; //to get the dimensions
					while (x[i] != ' ') {
						i++;
					}
					x = x.substr(7, i);
					createMatrix(&mtx, stoi(x));
					nodes = new int[mtx.dimension];
					sorted = new ForwardCheckingNode[mtx.dimension];
					createForwardCheckingNodes(sorted, mtx.dimension);
					continue;
				}
				if (x[0] == 'e') {
					int firstForwardCheckingNode, secondForwardCheckingNode;
					string tmp;
					int i = 2;
					while (x[i] != ' ')
						i++;
					tmp = x.substr(2, i);
					firstForwardCheckingNode = stoi(tmp);
					tmp = x.substr(i + 1, x.size());
					secondForwardCheckingNode = stoi(tmp);
					mtx.adj[firstForwardCheckingNode - 1][secondForwardCheckingNode - 1] = 1;
					mtx.adj[secondForwardCheckingNode - 1][firstForwardCheckingNode - 1] = 1;
					sorted[firstForwardCheckingNode - 1].numOfNeighbors++;
					sorted[secondForwardCheckingNode - 1].numOfNeighbors++;
				}
			}
		}
		if (mtx.dimension == 0) {
			cout << "Solution: 0 colors";
			getchar();
			getchar();
		}
		else
			FCandAC(mtx, sorted, 0);


		const sec duration = clock::now() - before;
		std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
		cout << "NUM OF STATES: " << counter1 << endl;
		getchar();
		getchar();

		input.close();
	}

}


