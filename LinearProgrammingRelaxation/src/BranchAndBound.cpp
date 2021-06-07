#include <iostream>					// for cout etc.
#include "BranchAndBound.h"
#include "Utillis.h"


void BranchAndBound::LDS(MDKP & mdkpProblem)
{
	// queue for traversing the node
	std::vector<Node> myQueue;	

	Node u, v;
	int n = mdkpProblem.getNumOfObjects();
	int m = mdkpProblem.getNumOfKnapsacks();


	u.level = -1;						
	u.profit = 0;
	u.weight.resize(m, 0);
	myQueue.push_back(u);


	// the maximum profit till this state
	int maxProfit = 0;				   

	while (myQueue.size() != 0)
	{
		// extract the next node 
		u = myQueue[0];
		myQueue.erase(myQueue.begin());
		u.right = true;


		/**

		// assign level 0
		if (u.level == -1)
		{
			v.level = 0;
		}

		// If there is nothing on next level
		if (u.level == n - 1)
			continue;
			*/

		// Not taking the item in knapsack 

		while (1)
		{
			/**
			u.upperBound = Utillis::FindUpperBound(mdkpProblem, u);
			if (u.upperBound > maxProfit)
			{
				if (!u.right)
					myQueue.push_back(u);
			}
			*/

			// assign level 0
			if (u.level == -1)
			{
				v.level = 0;
			}

			// If there is nothing on next level
			if (u.level == n - 1)
				break;

			v.level = u.level + 1;
			v.weight = Utillis::CalcWeight(mdkpProblem, u.weight, v.level);
			v.profit = u.profit + Utillis::CalcValue(mdkpProblem, v.level);

			
			// Don't take the item in knapsack
			if (u.right)
			{
				v.weight = u.weight;
				v.profit = u.profit;
			}

			if (!Utillis::CheckValidWeight(mdkpProblem, v.weight)) {

				break;
			}

			if (v.profit > maxProfit)
			{
				maxProfit = v.profit;
			}

			// Finding the upper bound to decide wether to stop or continue searching
			v.upperBound = Utillis::FindUpperBound(mdkpProblem, v);

			if (v.upperBound > maxProfit)
			{
				myQueue.push_back(v);
			}

			else {
				break;
			}

			u = v;
		}
	}


	//std::cout << maxProfit;
}
