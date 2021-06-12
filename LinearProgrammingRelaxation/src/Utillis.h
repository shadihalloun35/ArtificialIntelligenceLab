#pragma once
#include "MDKP.h"

struct Node
{
	int level;
	int profit;
	int upperBound;
	bool right;
	std::vector<int> weight;
};

class Utillis
{

public:

	static std::vector<int> CalcWeight(MDKP & mdkpProblem, std::vector<int> weight, int level);
	static int CalcValue(MDKP & mdkpProblem, int level);
	static bool CheckValidWeight(MDKP & mdkpProblem, std::vector<int> weights);
	static int FindUpperBound(MDKP & mdkpProblem, Node v, int heuristic);	
	static int UnlimitedSack(MDKP & mdkpProblem, Node v);
	//static int FractionalVariables(MDKP & mdkpProblem, Node v);
	static void InitRoot(MDKP & mdkpProblem, Node & u, int heuristic);
	static void KnapsackSorting(MDKP & mdkpProblem);
	static std::vector<int> ExtractWeights(MDKP & mdkpProblem);
	static std::vector<float> ExtractDensity(std::vector<int> myValues, std::vector<int> myWeights);


};