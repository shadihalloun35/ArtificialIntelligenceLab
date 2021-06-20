#include <iostream>					// for cout etc.
#include "BranchAndBound.h"
#include <algorithm>
#include "Utillis.h"
#define heuristic	2
 

std::vector<bool> BranchAndBound::LDSMDKP(MDKP & mdkpProblem)
{
	// queue for traversing the node
	std::vector<Node> myQueue;	
	Utillis::KnapsackSorting(mdkpProblem);
	std::vector<knapsack> knapsacks = mdkpProblem.getKnapsacks();
	std::vector<bool> currentObjects;


	Node u, v;
	int n = mdkpProblem.getNumOfObjects();
	int m = mdkpProblem.getNumOfKnapsacks();
	int wave = 0;
	u.currentObjects.resize(n, false);
	v.currentObjects.resize(n, false);

	Utillis::InitRoot(mdkpProblem, u, heuristic);
	myQueue.push_back(u);
	Node savedNode;

	// the maximum profit till this state
	int maxProfit = 0;				   


	while (myQueue.size() != 0)
	{
		// extract the next node 
		u = myQueue[0];

		if (wave != 0)
		{
			myQueue.erase(myQueue.begin());
		}

		u.right = true;

		while (1)
		{
		
			// If there is nothing on next level
			if (u.level == n)
				break;

			v.level = u.level + 1;
			v.right = false;
			v.currentObjects = u.currentObjects;

			// Don't take the item in knapsack
			if (u.right && wave != 0)
			{
				v.weight = u.weight;
				v.profit = u.profit;
				v.currentObjects[v.level - 1] = false;
			}

			// Take the item in knapsack
			else {
				v.weight = Utillis::CalcWeight(mdkpProblem, u.weight, v.level);
				v.profit = u.profit + Utillis::CalcValue(mdkpProblem, v.level);
				v.currentObjects[v.level - 1] = true;
			}
			
			if (!Utillis::CheckValidWeight(mdkpProblem, v.weight))
				break;

			if (v.profit > maxProfit)
			{
				maxProfit = v.profit;
				savedNode = v;
			}

			// Finding the upper bound to decide wether to stop or continue searching
			v.upperBound = Utillis::FindUpperBound(mdkpProblem, v, heuristic);

			if (v.upperBound > maxProfit)	
				myQueue.push_back(v);

			else 
				break;

			u = v;
		}

		wave = 1;
	}

	/**
	for (int i = 0 ; i < n; i++)
	{
		std::cout << knapsacks[0].values[i] << std::endl;
		std::cout << savedNode.currentObjects[i] << std::endl;

	}
	*/

	std::cout << "Optimal Value: " << maxProfit << std::endl;
	return savedNode.currentObjects;
}

void BranchAndBound::LDSCVRP(CVRP & cvrpProblem)
{
	MDKP myMDKPProblem;
	int diminsion = cvrpProblem.getDimension();
	Utillis::FillKnapsackDetails(cvrpProblem, myMDKPProblem);
	std::vector <std::vector<bool>> tours;
	std::vector<bool> unitedTours(diminsion, false);
	while (1)
	{
		std::vector<bool> tour = LDSMDKP(myMDKPProblem);
		tours.push_back(tour);
		unitedTours = Utillis::UniteTours(unitedTours, tour);
		Utillis::UpdateKnapsackDetails(myMDKPProblem, unitedTours);

		if (std::all_of(
			std::begin(unitedTours),
			std::end(unitedTours),
			[](bool i)
		{
			return i; 
		}
		)) {
			break;
		}
	}

	


}
