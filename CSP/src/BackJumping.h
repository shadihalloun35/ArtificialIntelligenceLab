#pragma once
#include "Base.h"
#include "Node.h"
#include <string>
#include <vector> 
using namespace std;

struct BackJumpingNode : Node
{
	vector<int> conflicts;
};

class BackJumping : public Base
{

public:
	static void ActivateBackJumping(string filePath);
	static bool tryColor(Matrix mtx, BackJumpingNode* nodes, int* org, int index);
	static void fixNeighbors(Matrix mtx, BackJumpingNode* nodes, int* org, int index);
	static void resetBackJumpingNodes(Matrix mtx, BackJumpingNode* nodes, int size);
	static void backjump(Matrix mtx, BackJumpingNode* nodes, int* org, int index, int des);
	static void printBackJumpingNodes(Matrix mtx, BackJumpingNode* nodes);
	static void backtracking(Matrix mtx, BackJumpingNode* nodes, int* org, int index);
	static bool sortByNeighbors(BackJumpingNode x, BackJumpingNode y);
	static void ordering(Matrix mtx, BackJumpingNode* sorted, int* nodes);
	static void createBackJumpingNodes(BackJumpingNode* nodes, int size);


};
