#pragma once
#include "Problem.h"
#include "Soulution.h"


struct Ant
{
	Soulution soulution;
	float pheromone;
	std::vector <Soulution> tabuSet;
};

class AntColonyOptimization
{

public:
	static void ActivateAntColonyOptimization(Problem & myProblem);
	static void InitAnts(Problem & myProblem);
	static void GenerateSolutions(Problem & myProblem, int i);
	static void AddToList(Soulution * nextSolution);
	static void AddToTabu(Ant & ant , Soulution & nextSolution);
	static float CalcProbability(Soulution * soulution, int i);
	static void UpdatePheromone();

private:
	static std::vector <Ant> ants;
	static std::vector <Soulution*> soulutions;
};

