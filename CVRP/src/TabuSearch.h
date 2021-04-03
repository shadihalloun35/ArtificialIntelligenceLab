#pragma once
#include "Problem.h"
#include "Soulution.h"
#include <unordered_set>

class TabuSearch
{
public:
	static void ActivateTabuSearch(Problem& myProblem);
	static void InsertToTabu(std::vector<std::vector<vec2>> soulotion);
	static bool Satisfiable(std::vector<std::vector<vec2>> soulotion);
	static void InitTabuSize();
	static void UpdateTabuSize();
	static int getTabuSize();


private:

	// declaring set for storing solutions in a faster way
	static unordered_set <std::vector<std::vector<vec2>>> tabuSet;
	static int tabuSize;

};

