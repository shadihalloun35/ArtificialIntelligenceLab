#include "Utillis.h"


std::vector<float> Utillis::CalcWeight(MDKP & mdkpProblem, std::vector<float> previousWeights, int level)
{
	int numOfKnapsacks = mdkpProblem.getNumOfKnapsacks();
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	std::vector<float> weights(numOfKnapsacks, 0);
	int index = level - 1;

	for (int i = 0; i < numOfKnapsacks; i++)
	{
		weights[i] = previousWeights[i] + knapsacks[i].weights[index];
	}

	return weights;
}

int Utillis::CalcValue(MDKP & mdkpProblem, int level)
{
	int numOfKnapsacks = mdkpProblem.getNumOfKnapsacks();
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	int index = level - 1;
	int value = knapsacks[0].values[index];
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

int Utillis::FindUpperBound(MDKP & mdkpProblem, Node v, int heuristic)
{
	switch(heuristic)
	{
	case 1:
		return Utillis::UnlimitedSack(mdkpProblem, v);
		break;

	case 2:
		//return Utillis::FractionalVariables(mdkpProblem, v);
		break;
	}
}

int Utillis::UnlimitedSack(MDKP & mdkpProblem, Node v)
{
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	int m = mdkpProblem.getNumOfKnapsacks();
	int n = mdkpProblem.getNumOfObjects();


	for (int i = 0; i < m; i++)
	{
		if (v.weight[i] >= knapsacks[i].capacity)
			return 0;
	}

	int upperBound = v.profit;

	// start including items 
	int j = v.level;

	while (j < n)
	{
		upperBound += knapsacks[0].values[j];
		j++;
	}

	return upperBound;
}

/**
int Utillis::FractionalVariables(MDKP & mdkpProblem, Node v)
{
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	int m = mdkpProblem.getNumOfKnapsacks();
	int n = mdkpProblem.getNumOfObjects();


	for (int i = 0; i < m; i++)
	{
		if (v.weight[i] >= knapsacks[i].capacity)
			return 0;
	}

	// initialize bound on profit by current profit
	int upperBound = v.profit;

	std::vector <int> values = knapsacks[0].values;
	std::vector <int> weights = 
	// start including items from index 1 more to current
	// item index
	int j = u.level + 1;
	int totweight = u.weight;

	// checking index condition and knapsack capacity
	// condition
	while ((j < n) && (totweight + arr[j].weight <= W))
	{
		totweight += arr[j].weight;
		profit_bound += arr[j].value;
		j++;
	}

	// If k is not n, include last item partially for
	// upper bound on profit
	if (j < n)
		profit_bound += (W - totweight) * arr[j].value /
		arr[j].weight;

	return profit_bound;
}
*/

void Utillis::InitRoot(MDKP & mdkpProblem, Node & u, int heuristic)
{
	int n = mdkpProblem.getNumOfObjects();
	int m = mdkpProblem.getNumOfKnapsacks();

	u.level = 0;
	u.profit = 0;
	u.weight.resize(m, 0);
	u.upperBound = Utillis::FindUpperBound(mdkpProblem, u, heuristic);
}

/**
bool cmp(Item a, Item b)
{
	double r1 = (double)a.value / a.weight;
	double r2 = (double)b.value / b.weight;
	return r1 > r2;
}
*/