#pragma once
#include "Base.h"
#include "Node.h"
#include <vector> 
using namespace std;

class Objective : public Base
{

public:
	static void Retreat(Node* nodes, int* oldColors, int size);
	static void SwapCand(vector<int>* cand1, vector<int>* cand2);
	static bool ContainsInCand(vector<int>* cand1, int index);
	static void PushConflicted(Matrix* mtx, Node* nodes, vector<int>* cand1, vector<int>* cand2, int color);
	static void ChangeColor(vector<int>* cand1, Node* nodes, int color);
	static int KempeChain(Matrix* mtx, Node* nodes);
	static void CopyColors(Node* nodes, int* oldColors, int size);
	static void FixColorsInGraph(int size, Node* nodes, int colorDeleted, int maxColors);
	static int CalcCi(Matrix* mtx, Node* nodes, int size, int colors);
	static int GreedyAlgorithm(Matrix* mtx, Node* nodes);
	static void SimulatedAnnealing(Matrix mtx, Node* nodes, std::chrono::duration < double> timeAllowed);
	static void PrintObjectiveNodes(Matrix & mtx, Node* nodes);
	static void CreateObjectiveNodes(Node* nodes, int size);
	static void ActivateObjective();
};

