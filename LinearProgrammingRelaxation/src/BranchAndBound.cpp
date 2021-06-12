#include <iostream>					// for cout etc.
#include "BranchAndBound.h"
#include "Utillis.h"
#define heuristic	1
 

void BranchAndBound::LDS(MDKP & mdkpProblem)
{
	// queue for traversing the node
	std::vector<Node> myQueue;	
	Utillis::KnapsackSorting(mdkpProblem);

	Node u, v;
	int n = mdkpProblem.getNumOfObjects();
	int m = mdkpProblem.getNumOfKnapsacks();

	/**
	std::cout << n << std::endl;
	std::cout << m << std::endl;

	std::cout << mdkpProblem.getKnapsacks()[0].values[0] << std::endl;
	std::cout << mdkpProblem.getKnapsacks()[0].values[mdkpProblem.getKnapsacks()[0].values.size()-1] << std::endl;
	std::cout << mdkpProblem.getKnapsacks()[0].capacity << std::endl;
	std::cout << mdkpProblem.getKnapsacks()[0].weights[0] << std::endl;
	std::cout << mdkpProblem.getKnapsacks()[0].weights[mdkpProblem.getKnapsacks()[0].weights.size() - 1] << std::endl;


	std::cout << mdkpProblem.getKnapsacks()[1].values[0] << std::endl;
	std::cout << mdkpProblem.getKnapsacks()[1].values[mdkpProblem.getKnapsacks()[0].values.size() - 1] << std::endl;
	std::cout << mdkpProblem.getKnapsacks()[1].capacity << std::endl;
	std::cout << mdkpProblem.getKnapsacks()[1].weights[0] << std::endl;
	std::cout << mdkpProblem.getKnapsacks()[1].weights[mdkpProblem.getKnapsacks()[0].weights.size() - 1] << std::endl;
	*/
	

	int wave = 0;

	Utillis::InitRoot(mdkpProblem, u, heuristic);
	myQueue.push_back(u);
	//Node savedNode;

	// the maximum profit till this state
	int maxProfit = 0;				   


	while (myQueue.size() != 0)
	{
		//std::cout << myQueue.size() << std::endl;
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

			// Don't take the item in knapsack
			if (u.right && wave != 0)
			{
				v.weight = u.weight;
				v.profit = u.profit;
			}

			// Take the item in knapsack
			else {
				v.weight = Utillis::CalcWeight(mdkpProblem, u.weight, v.level);
				v.profit = u.profit + Utillis::CalcValue(mdkpProblem, v.level);
			}
			
			if (!Utillis::CheckValidWeight(mdkpProblem, v.weight))
				break;

			if (v.profit > maxProfit)
			{
				maxProfit = v.profit;
				//savedNode = v;
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
	std::cout << "Level: " << savedNode.level << std::endl;
	std::cout << "Value: " << savedNode.profit << std::endl;
	std::cout << "Upper bound: " << savedNode.upperBound << std::endl;
	std::cout << "Weight1: " << savedNode.weight[0] << std::endl;
	std::cout << "Weight2: " << savedNode.weight[1] << std::endl;
	*/
	std::cout << "Optimal Value: " << maxProfit << std::endl;
}
