#pragma once
#include "MDKP.h"

struct Node
{
	int level;
	int profit;
	int upperBound;
	bool right;
	std::vector<float> weight;
};

class Utillis
{

public:

	static std::vector<float> CalcWeight(MDKP & mdkpProblem, std::vector<float> weight, int level);
	static int CalcValue(MDKP & mdkpProblem, int level);
	static bool CheckValidWeight(MDKP & mdkpProblem, std::vector<float> weights);
	static int FindUpperBound(MDKP & mdkpProblem, Node v, int heuristic);	
	static int UnlimitedSack(MDKP & mdkpProblem, Node v);
	//static int FractionalVariables(MDKP & mdkpProblem, Node v);
	static void InitRoot(MDKP & mdkpProblem, Node & u, int heuristic);
};