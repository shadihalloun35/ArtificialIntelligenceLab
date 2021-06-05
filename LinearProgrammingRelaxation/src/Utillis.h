#pragma once
#include "MDKP.h"

struct Node
{
	int level;
	int profit;
	int upperBound;
	float weight;
};

class Utillis
{

public:

	static float CalcWeight(MDKP & mdkpProblem,int level);
	static float CalcValue(MDKP & mdkpProblem, int level);

	
};