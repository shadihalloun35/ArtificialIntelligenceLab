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
		return Utillis::FractionalVariables(mdkpProblem, v);
		break;
	}
	return 0;
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


int Utillis::FractionalVariables(MDKP & mdkpProblem, Node v)
{
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	int m = mdkpProblem.getNumOfKnapsacks();
	int n = mdkpProblem.getNumOfObjects();
	std::vector<int> weights(m, 0);


	for (int i = 0; i < m; i++)
	{
		if (v.weight[i] >= knapsacks[i].capacity)
			return 0;
	}

	int upperBound = v.profit;

	// start including items 
	int j = v.level;

	if (j + 1 < n)
		weights = Utillis::CalcWeight(mdkpProblem, weights, j+1);

	while (j < n && Utillis::CheckValidWeight(mdkpProblem, weights))
	{
		if ( j + 2 < n)
			weights = Utillis::CalcWeight(mdkpProblem, weights, j+2);
		upperBound += knapsacks[0].values[j];
		j++;
	}

	if (j < n)
	{
		int chosenWeight = weights[0] - knapsacks[0].capacity;
		int index = 0;
		for (int i = 0; i < m; i++)
		{
			int currentWeight = weights[i] - knapsacks[i].capacity;
			if (chosenWeight <= currentWeight)
			{
				chosenWeight = currentWeight;
				index = i;
			}

		}

		upperBound += (knapsacks[index].capacity - weights[index]) * knapsacks[index].values[j] / knapsacks[index].weights[j];
	}

	return upperBound;
}


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

void Utillis::FillKnapsackDetails(CVRP & cvrpProblem, MDKP & myMDKPProblem)
{
	knapsack currentKnapsack;
	std::vector<knapsack> myKnapsacks;
	int diminsion = cvrpProblem.getDimension();
	int carCapacity = cvrpProblem.getCapacity();
	std::vector<vec2> cities = cvrpProblem.getCoordinates();
	std::vector<int> myWeights;
	std::vector<int> myValues;

	for (int i = 1; i < diminsion; i++)
	{
		myValues.push_back(1);
		myWeights.push_back(cities[i].demand);
	}

	currentKnapsack.values = myValues;
	currentKnapsack.weights = myWeights;
	currentKnapsack.capacity = carCapacity;
	myKnapsacks.push_back(currentKnapsack);
	myMDKPProblem.setKnapsacks(myKnapsacks);
	myMDKPProblem.setNumOfKnapsacks(1);
	myMDKPProblem.setNumOfObjects(diminsion-1);

}

void Utillis::UpdateKnapsackDetails(MDKP & myMDKPProblem, std::vector<bool> tour)
{
	knapsack currentKnapsack;
	std::vector<knapsack> myKnapsacks = myMDKPProblem.getKnapsacks();
	knapsack myKnapsack = myKnapsacks[0];
	int diminsion = tour.size();
	std::vector<int> myValues(diminsion, 0);

	

	for (int i = 0; i < diminsion; i++)
	{
		if (!tour[i])
		{
			myValues[i] = 1;
		}
	}

	

	myKnapsack.values = myValues;
	myKnapsacks[0] = myKnapsack;
	myMDKPProblem.setKnapsacks(myKnapsacks);

}

std::vector<bool> Utillis::UniteTours(std::vector<bool> unitedTours, std::vector<bool> tour)
{
	int diminsion = tour.size();
	std::vector<bool> newTour(diminsion, false);
	for (int i = 0; i < diminsion; i++)
	{
		if (unitedTours[i]||tour[i])
			newTour[i] = true;
	}

	return newTour;
}

std::vector<std::pair<int, int>> Utillis::KnapsackSorting(MDKP & mdkpProblem)
{
	std::vector<std::pair<int, int>> vp;
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

	return vp;
}
