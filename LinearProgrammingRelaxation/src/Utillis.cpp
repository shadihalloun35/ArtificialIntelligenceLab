#include "Utillis.h"
#include <algorithm>    // std::sort
#include <iostream>		// for cout etc.


std::vector<int> Utillis::CalcWeight(MDKP & mdkpProblem, std::vector<int> previousWeights, int level)
{
	int numOfKnapsacks = mdkpProblem.getNumOfKnapsacks();
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	std::vector<int> weights(numOfKnapsacks, 0);
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

bool Utillis::CheckValidWeight(MDKP & mdkpProblem, std::vector<int> weights)
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

std::vector<int> Utillis::ExtractWeights(MDKP & mdkpProblem)
{
	int n = mdkpProblem.getNumOfKnapsacks();
	int m = mdkpProblem.getNumOfObjects();
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	std::vector<int> myWeights;

	for (int i = 0; i < m; i++)
	{
		int currentWeight = 0;
		for (int j = 0; j < n; j++)
		{
			currentWeight += knapsacks[j].weights[i];
		}
		myWeights.push_back(currentWeight);
	}
	
	return myWeights;
}

std::vector<float> Utillis::ExtractDensity(std::vector<int> myValues, std::vector<int> myWeights)
{
	std::vector<float> density;
	int n = myValues.size();
	for (int i = 0; i < n; i++)
	{
		density.push_back((float)myValues[i]/ myWeights[i]);
	}

	return density;
}

void Utillis::KnapsackSorting(MDKP & mdkpProblem)
{
	std::vector<std::pair<int, int> > vp;
	std::vector<int> myValues = mdkpProblem.getKnapsacks()[0].values;
	std::vector<int> newValues;
	std::vector<knapsack> newKnapsacks;
	std::vector<int> myWeights = ExtractWeights(mdkpProblem);
	std::vector<float> density = ExtractDensity(myValues, myWeights);
	int m = mdkpProblem.getNumOfKnapsacks();
	int n = myValues.size();


	for (int i = 0; i < n; ++i) {
		vp.push_back(std::make_pair(density[i], i));
	}
	std::sort(vp.rbegin(), vp.rend());

	for (int i = 0; i < n; i++) {
		newValues.push_back(myValues[vp[i].second]);
		
	}

	for (int i = 0; i < m; i++) {
		knapsack myKnapsack;
		std::vector<int> newWeights;
		std::vector<int> currentWeights = mdkpProblem.getKnapsacks()[i].weights;
		int currentCapacity = mdkpProblem.getKnapsacks()[i].capacity;
		for (int j = 0; j < n; j++) {
			newWeights.push_back(currentWeights[vp[j].second]);
		}
		myKnapsack.capacity = currentCapacity;
		myKnapsack.values = newValues;
		myKnapsack.weights = newWeights;
		newKnapsacks.push_back(myKnapsack);
		

	}

	mdkpProblem.setKnapsacks(newKnapsacks);


}



/**
bool cmp(Item a, Item b)
{
	double r1 = (double)a.value / a.weight;
	double r2 = (double)b.value / b.weight;
	return r1 > r2;
}
*/