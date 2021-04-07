#pragma once
#include "Problem.h"
#include "Soulution.h"

class TabuSearch
{
public:
	static void ActivateTabuSearch(Problem& myProblem);
	static void InsertToTabu(std::vector<std::vector<vec2>> soulotion);
	static bool Satisfiable(std::vector<std::vector<vec2>> soulotion);
	static void InitTabuSize();
	static void UpdateTabuSize();
	static unsigned int getTabuSize();


private:

	static std::vector <std::vector<std::vector<vec2>>> tabuSet;
	static unsigned int tabuSize;

};

