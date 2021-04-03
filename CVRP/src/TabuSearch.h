#pragma once
#include "Problem.h"
#include "Soulution.h"

class TabuSearch
{
public:
	static void ActivateTabuSearch(Problem& myProblem);
	static int InitTabuSize();
	static int UpdateTabuSize(int n);
};

