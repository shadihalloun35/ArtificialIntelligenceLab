#pragma once
#include "MDKP.h"
#include "CVRP.h"

class BranchAndBound
{

public:
	static std::vector<bool> LDSMDKP(MDKP & mdkpProblem);
	static void LDSCVRP(CVRP & cvrpProblem);

};