#include <iostream>					// for cout etc.
#include "BranchAndBound.h"
#include <algorithm>
#include "Utillis.h"
#define heuristic	1
 

std::vector<bool> BranchAndBound::LDSMDKP(MDKP & mdkpProblem)
{
	// queue for traversing the node
	std::vector<knapsack> myKnapsacks = mdkpProblem.getKnapsacks();
	knapsack myKnapsack = myKnapsacks[0];

	std::vector<std::pair<int, int>> vp;
	std::vector<Node> myQueue;	
	vp = Utillis::KnapsackSorting(mdkpProblem);
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
				v.currentObjects[vp[v.level - 1].second] = false;
			}

			// Take the item in knapsack
			else {
				v.weight = Utillis::CalcWeight(mdkpProblem, u.weight, v.level);
				v.profit = u.profit + Utillis::CalcValue(mdkpProblem, v.level);
				v.currentObjects[vp[v.level - 1].second] = true;
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


	myKnapsacks[0] = myKnapsack;
	mdkpProblem.setKnapsacks(myKnapsacks);

	std::cout << "Optimal Value: " << maxProfit << std::endl;
	return savedNode.currentObjects;
}

void BranchAndBound::LDSCVRP(CVRP & cvrpProblem)
{
	bool flag = true;
	MDKP myMDKPProblem;
	int diminsion = cvrpProblem.getDimension()-1;
	Utillis::FillKnapsackDetails(cvrpProblem, myMDKPProblem);
	std::vector <std::vector<bool>> tours;
	std::vector<bool> unitedTours(diminsion, false);

	while (flag)
	{
		std::vector<bool> tour = LDSMDKP(myMDKPProblem);
		tour = Utillis::FixTours(unitedTours, tour);
		
		tours.push_back(tour);
		unitedTours = Utillis::UniteTours(unitedTours, tour);
		Utillis::UpdateKnapsackDetails(myMDKPProblem, unitedTours);

		
		for (size_t i = 0; i < unitedTours.size(); i++)
		{

			if (!unitedTours[i])
				break;

			if (i == unitedTours.size() - 1)
				flag = false;
		}				
	}

}
