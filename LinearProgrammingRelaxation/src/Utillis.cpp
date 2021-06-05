#include "Utillis.h"

std::vector<float> Utillis::CalcWeight(MDKP & mdkpProblem, std::vector<float> previousWeights, int level)
{
	int numOfKnapsacks = mdkpProblem.getNumOfKnapsacks();
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	std::vector<float> weights(numOfKnapsacks, 0);

	for (int i = 0; i < numOfKnapsacks; i++)
	{
		weights[i] = previousWeights[i] + knapsacks[i].weights[level];
	}

	return weights;
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

bool Utillis::CheckValidWeight(MDKP & mdkpProblem, std::vector<float> weights)
{
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	int m = mdkpProblem.getNumOfKnapsacks();
	
	for (int i = 0; i < m; i++)
	{
		if (weights[i] > knapsacks[i].capacity)
			return false;
	}

	return true;
}

int Utillis::FindUpperBound(MDKP & mdkpProblem, Node u)
{
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	int m = mdkpProblem.getNumOfKnapsacks();
	int n = mdkpProblem.getNumOfObjects();


	for (int i = 0; i < m; i++)
	{
		if (u.weight[i] >= knapsacks[i].capacity)
			return 0;
	}

	int upperBound = u.profit;

	// start including items 
	int j = u.level + 1;

	while (j < n)
	{
		for (int i = 0; i < m; i++)
		{
			upperBound += knapsacks[i].values[j];
		}
		j++;
	}

	return upperBound;
}
