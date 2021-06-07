#pragma once
#include "MDKP.h"

struct Node
{
	int level;
	int profit;
	int upperBound;
	bool left = false;
	bool right = false;
	std::vector<float> weight;
};

class Utillis
{

public:

	static std::vector<float> CalcWeight(MDKP & mdkpProblem, std::vector<float> weight, int level);
	static float CalcValue(MDKP & mdkpProblem, int level);
	static bool CheckValidWeight(MDKP & mdkpProblem, std::vector<float> weights);
	static int FindUpperBound(MDKP & mdkpProblem, Node u);	
};