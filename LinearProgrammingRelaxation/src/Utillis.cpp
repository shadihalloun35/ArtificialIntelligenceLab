#include "Utillis.h"

float Utillis::CalcWeight(MDKP & mdkpProblem, int level)
{
	float weight = 0;
	int numOfKnapsacks = mdkpProblem.getNumOfKnapsacks();
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();

	for (int i = 0; i < numOfKnapsacks; i++)
	{
		weight += knapsacks[i].weights[level];
	}

	return weight;
}

float Utillis::CalcValue(MDKP & mdkpProblem, int level)
{
	float value = 0;
	int numOfKnapsacks = mdkpProblem.getNumOfKnapsacks();
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();

	for (int i = 0; i < numOfKnapsacks; i++)
	{
		value += knapsacks[i].values[level];
	}

	return value;

}
