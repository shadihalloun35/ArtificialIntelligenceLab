#pragma once
#include "Base.h"
#include "Node.h"
#include <vector> 
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
using namespace std;

class Hybrid : public Base
{

public:
	static void GeneticAlgorithm(Matrix* mtx, std::chrono::duration <double> timeAllowed);
	static void GreedyAlgorithm(Matrix* mtx, Node* nodes);
	static void PrintHybridNodes(Matrix & mtx, Node* nodes);
	static void CreateHybridNodes(Node* nodes, int size);
	static void ActivateHybrid();
};
