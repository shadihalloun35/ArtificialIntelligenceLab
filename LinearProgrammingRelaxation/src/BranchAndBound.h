#pragma once
#include "MDKP.h"
#include "CVRP.h"

class BranchAndBound
{

public:
	static int LDSMDKP(MDKP mdkpProblem);
	static int LDSCVRP(CVRP cvrpProblem);

};