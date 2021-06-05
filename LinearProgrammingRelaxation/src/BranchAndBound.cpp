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

		// assign level 0
		if (u.level == -1)
			v.level = 0;

		// If there is nothing on next level
		if (u.level == n - 1)
			continue;

		v.level = u.level + 1;

		v.weight = Utillis::CalcWeight(mdkpProblem, u.weight , v.level);
		v.profit = u.profit + Utillis::CalcValue(mdkpProblem, v.level);
		 
		if (Utillis::CheckValidWeight(mdkpProblem,v.weight) && v.profit > maxProfit)
		{
			maxProfit = v.profit;
		}

		// Finding the upper bound to decide wether to stop or continue searching
		v.upperBound = Utillis::FindUpperBound(mdkpProblem,v);

		if (v.upperBound > maxProfit)
			myQueue.push_back(v);
	}



}
