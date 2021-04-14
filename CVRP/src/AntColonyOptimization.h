#pragma once
#include "Problem.h"
#include "Soulution.h"
#include "Edge.h"



struct Ant
{
	Soulution soulution;								// the soulution the ant found	
	float pheromone;									// the paramater Q in the lecture
	std::vector <vec2> tabuSet;							// this list has the city this ant has visited
	std::vector <Edge*> edges;							// this is the list of edges this ant has used
};

class AntColonyOptimization
{

public:

	static void ActivateAntColonyOptimization(Problem & myProblem);
	static void InitAnts(Problem & myProblem);
	static void GenerateSolutions(Problem & myProblem , int i);
	static std::vector<vec2> FindPermutation(Problem & myProblem , int i);
	static void AddToList(vec2 city1, vec2 city2, int i);
	static void AddToTabu(Ant & ant , vec2 city);
	static std::vector<float> CalcProbability(Problem & myProblem , vec2 currentCity , int i);
	static void UpdatePheromone();

private:
	static std::vector <Ant> ants;						// this is the list of ants we have used
	static std::vector <Edge> edges;							// this is the list of edges that all ants have visited
};

