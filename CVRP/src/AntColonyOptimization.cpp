#include "AntColonyOptimization.h"
#include "Utillis.h"
#define MAXSEARCHES		1000
#define NumberOfAnts	1000
#define Alpha			0.7
#define Beta			0.3

// definition for our static variable
std::vector <Ant> AntColonyOptimization::ants;

void AntColonyOptimization::ActivateAntColonyOptimization(Problem & myProblem)
{
	InitAnts(myProblem);
	Soulution bestSolution;
	float BestSolutionCost = RAND_MAX;

	for (int k = 0; k < MAXSEARCHES; k++)
	{
		for (int i = 0; i < NumberOfAnts; i++) 
		{
			DaemonActions(myProblem,i);

			if (Utillis::CalcTourDistance(ants[i].soulution.getTrucksTour()) < BestSolutionCost)
			{
				bestSolution = ants[i].soulution;
				BestSolutionCost = bestSolution.getDistance();
			}
		}

		UpdatePheromone();
	}
}

void AntColonyOptimization::InitAnts(Problem & myProblem)
{
	for (int i = 0; i < NumberOfAnts; i++) {
		Ant ant;
		Soulution soulution;
		std::vector<std::vector<vec2>> bestSolution = Utillis::GenerateInitialSolution(myProblem);		// generating initial soulution
		soulution.setNumOfCarsAllowed(myProblem.getNumOfTrucks());
		soulution.setPermutation(myProblem.getCoordinates());
		float solutionCost = Utillis::CalcTourDistance(bestSolution);
		Utillis::UpdateSolution(soulution, bestSolution, solutionCost);									// updating the soulution
		ant.soulution = soulution;
		ant.pheromone = soulution.getDistance();
		ant.visibility = 1/ soulution.getDistance();
		ants.push_back(ant);
	}
}

void AntColonyOptimization::DaemonActions(Problem & myProblem,int i)
{
	myProblem.setCoordinates(ants[i].soulution.getPermutation());
	std::vector<std::vector<vec2>> nextSolution = Utillis::getNeighbor(myProblem);

	if ((double)rand() / (RAND_MAX) <= calcProbability())
	{
		float solutionCost = Utillis::CalcTourDistance(nextSolution);									// saving the new soulution
		ants[i].soulution.setPermutation(myProblem.getCoordinates());						
		Utillis::UpdateSolution(ants[i].soulution, nextSolution, solutionCost);
	}
	
}

float AntColonyOptimization::calcProbability()
{
	return 0.0f;
}

void AntColonyOptimization::UpdatePheromone()
{
	for (int i = 0; i < NumberOfAnts; i++)
	{
		
	}
}


