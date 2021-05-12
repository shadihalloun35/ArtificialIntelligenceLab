#pragma once

#include "Base.h"
#include "Node.h"
#include <vector> 
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
#include <string>

using namespace std;


class Feasibility : public Base
{

public:
	static void PrintFeasibilityNodes(Matrix & mtx, Node* nodes);
	static int calcConflict(Matrix & mtx, Node* nodes, int index, int color);
	static void tryFix(Matrix & mtx, Node* nodes, int maxColors);
	static void fixGraph(Matrix & mtx, Node* nodes, int color);
	static bool isLegal(Matrix mtx, Node* nodes);
	static void greedyAlgorithm(Matrix* mtx, Node* nodes);
	static void CreateFeasibilityNodes(Node* nodes, int size);
	static void MinimalConflict(Matrix & mtx, Node* nodes, std::chrono::duration<double> timeAllowed);
	static void ActivateFeasibility(string filePath, int timeAllowed);
};
