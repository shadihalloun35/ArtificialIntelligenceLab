#include "AntColonyOptimization.h"
#include "Utillis.h"
#include <math.h>       /* pow */
#define MAXSEARCHES		1000
#define NumberOfAnts	1000
#define Alpha			1.0f
#define Beta			5.0f
#define evaporation		0.5f

// definition for our static variable
std::vector <Ant> AntColonyOptimization::ants;
std::vector <Soulution*> AntColonyOptimization::soulutions;

void AntColonyOptimization::ActivateAntColonyOptimization(Problem & myProblem)
{
	InitAnts(myProblem);
	Soulution bestSoulution;
	float BestSolutionCost = RAND_MAX;

	for (int k = 0; k < MAXSEARCHES; k++)
	{
		for (int i = 0; i < NumberOfAnts; i++)						 // generate solutions
		{
			GenerateSolutions(myProblem,i);																  

			if (Utillis::CalcTourDistance(ants[i].soulution.getTrucksTour()) < BestSolutionCost)
			{
				bestSoulution = ants[i].soulution;
				BestSolutionCost = bestSoulution.getDistance();
			}
			//std::cout << ants[i].soulution;

		}

		UpdatePheromone();											 // updating the pheromone
	}

	std::cout << bestSoulution << std::endl;
}

void AntColonyOptimization::InitAnts(Problem & myProblem)
{
	for (int i = 0; i < NumberOfAnts; i++) {
		Ant ant;
		Soulution *soulution = &ant.soulution;
		std::vector<std::vector<vec2>> bestSolution = Utillis::GenerateInitialSolution(myProblem);			// generating initial soulution
		soulution->setNumOfCarsAllowed(myProblem.getNumOfTrucks());
		soulution->setPermutation(myProblem.getCoordinates());
		float solutionCost = Utillis::CalcTourDistance(bestSolution);
		Utillis::UpdateSolution(*soulution, bestSolution, solutionCost);										// updating the soulution
		ant.soulution = *soulution;
		ant.pheromone = 10; ////erj3 3leha
		ant.tabuSet.push_back(*soulution);
		AddToList(soulution);
		ants.push_back(ant);
	}

	//std::cout << ants.size();

}

void AntColonyOptimization::GenerateSolutions(Problem & myProblem, int i)
{
	Soulution *soulution = &ants[i].soulution;
	myProblem.setCoordinates(ants[i].soulution.getPermutation());
	std::vector<std::vector<vec2>> nextSolution = Utillis::getNeighbor(myProblem);	
	float solutionCost = Utillis::CalcTourDistance(nextSolution);											// saving the new soulution
	soulution->setNumOfCarsAllowed(myProblem.getNumOfTrucks());
	Utillis::UpdateSolution(*soulution, nextSolution, solutionCost);

	if ((double)rand() / (RAND_MAX) <= CalcProbability(soulution, i))
	{
		//std::cout << "h";
		soulution->setPermutation(myProblem.getCoordinates());
		ants[i].soulution = *soulution;
		AddToList(soulution);
		AddToTabu(ants[i], *soulution);
	}
}

void AntColonyOptimization::AddToList(Soulution * nextSolution)
{
	if (std::find(soulutions.begin(), soulutions.end(), nextSolution) == soulutions.end())
	{
		soulutions.push_back(nextSolution);
	}
}

void AntColonyOptimization::AddToTabu(Ant & ant , Soulution & nextSolution)
{
	if (std::find(ant.tabuSet.begin(), ant.tabuSet.end(), nextSolution) == ant.tabuSet.end())
	{
		ant.tabuSet.push_back(nextSolution);
	}
}

float AntColonyOptimization::CalcProbability(Soulution * soulution, int i)
{
	if (std::find(ants[i].tabuSet.begin(), ants[i].tabuSet.end(), *soulution) != ants[i].tabuSet.end())
	{
		return 0;
	}

	float numerator = pow(soulution->getPheromone(), Alpha) * pow(soulution->getVisibility(), Beta);
	float denominator = 0;
	for (size_t i = 0; i < soulutions.size(); i++)
	{
		if (std::find(ants[i].tabuSet.begin(), ants[i].tabuSet.end(), *soulution) == ants[i].tabuSet.end())
			denominator += pow(soulutions[i]->getPheromone(), Alpha) * pow(soulutions[i]->getVisibility(), Beta);
	}

	return numerator/ denominator;
}

void AntColonyOptimization::UpdatePheromone()
{
	for (size_t i = 0; i < soulutions.size(); i++)
	{
		float pheromone = 0;
		for (int k = 0; k < NumberOfAnts; k++)
		{
			if (std::find(ants[k].tabuSet.begin(), ants[k].tabuSet.end(), *soulutions[i]) == ants[k].tabuSet.end())
				pheromone += ants[k].pheromone / soulutions[i]->getDistance();

		}
		pheromone = pheromone + soulutions[i]->getPheromone() * (1 - evaporation);
		//std::cout << soulutions[i].getPheromone() << std::endl;
		soulutions[i]->setPheromone(pheromone);
	}
}


