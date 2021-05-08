#pragma once
#include "Base.h"
#include "Node.h"
#include <vector> 
using namespace std;

struct ForwardCheckingNode : Node
{
	vector<int> domain;
};

struct Deleted
{
	ForwardCheckingNode node1;
	ForwardCheckingNode node2;
	int color;
};

class ForwardChecking : public Base
{
public:
	static void ActivateForwardChecking();
	static bool tryColor(Matrix mtx, ForwardCheckingNode* nodes, int index, vector<Deleted>* deleted);
	static void fixConsistency(ForwardCheckingNode* nodes, int index1, int index2, int color, vector<Deleted>* deleted);
	static void printDeletedForwardCheckingNodes(vector<Deleted> x);
	static bool checkAvailabilty(ForwardCheckingNode* nodes, int index, int color);
	static void deleteColorFromDomain(ForwardCheckingNode* nodes, int index, int color);
	static void fixNeighbors(Matrix mtx, ForwardCheckingNode* nodes, int index, vector<Deleted>* deleted);
	static int checkConsistency(Matrix mtx, ForwardCheckingNode* nodes, int source, int index, vector<Deleted>* deleted);
	static bool arcConsistency(Matrix mtx, ForwardCheckingNode* nodes, int source, vector<Deleted>* deleted);
	static void forwardChecking(Matrix mtx, ForwardCheckingNode* nodes, int index, int color, vector<Deleted>* deleted);
	static void resetForwardCheckingNodes(Matrix mtx, ForwardCheckingNode* nodes, int size);
	static void printForwardCheckingNodes(Matrix mtx, ForwardCheckingNode* nodes);
	static int nextForwardCheckingNode(Matrix mtx, ForwardCheckingNode* nodes, int index);
	static void FCandAC(Matrix mtx, ForwardCheckingNode* nodes, int index);
	static bool sortByNeighbors(ForwardCheckingNode x, ForwardCheckingNode y);
	static void ordering(Matrix mtx, ForwardCheckingNode* sorted, int* nodes);
	static void createForwardCheckingNodes(ForwardCheckingNode* nodes, int size);

};
