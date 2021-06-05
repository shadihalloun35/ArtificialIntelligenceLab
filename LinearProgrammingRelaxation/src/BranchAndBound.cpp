#include "BranchAndBound.h"
#include "Utillis.h"


void BranchAndBound::LDS(MDKP & mdkpProblem)
{
	std::vector<Node> myQueue;			// queue for traversing the node
	Node u, v;
	int n = mdkpProblem.getNumOfObjects();


	u.level = -1;						// dummy node at starting
	u.profit = u.weight = 0;
	myQueue.push_back(u);


	int maxProfit = 0;				   // the maximum profit till this state

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

		v.weight = u.weight + Utillis::CalcWeight(mdkpProblem, v.level);
		v.profit = u.profit + Utillis::CalcValue(mdkpProblem, v.level);
	}


}
