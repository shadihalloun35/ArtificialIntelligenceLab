#pragma once
#include "Problem.h"
#include "Soulution.h"


struct Ant
{
	Soulution soulution;
	float pheromone;
	float visibility;
};

class AntColonyOptimization
{

public:
	static void ActivateAntColonyOptimization(Problem& myProblem);
	static void InitAnts(Problem & myProblem);
	static void DaemonActions(Problem & myProblem, int i);
	static float calcProbability();
	static void UpdatePheromone();


	//static void InsertToTabu(std::vector<std::vector<vec2>> soulotion);
	//static bool Satisfiable(std::vector<std::vector<vec2>> soulotion);
	//static void InitTabuSize();
	//static void UpdateTabuSize();
	//static unsigned int getTabuSize();


private:
	static std::vector <Ant> ants;
};

