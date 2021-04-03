#include <iostream>					// for cout etc.
#include "SimulatedAnnealing.h"
#include "Utillis.h"
#include <algorithm>	// for random_shuffle
#include <math.h>       /* exp */
#define MAXSEARCHES		1000000


void SimulatedAnnealing::ActivateSimulatedAnnealing(Problem& myProblem)
{
	float tempreture = InitTemparature();
	std::vector<std::vector<vec2>> currentSolution = Utillis::GenerateInitialSolution(myProblem);		// generating initial solution
	std::vector<std::vector<vec2>> bestSolution = currentSolution;										// saving best solution so far
	Soulution mySoulution(bestSolution);																// creating our solution
	mySoulution.setNumOfCarsAllowed(myProblem.getNumOfTrucks());										
	float solutionCost = 0;

	for (int k = 0; k < MAXSEARCHES; k++)
	{
		std::vector<vec2> savedCoordinates = myProblem.getCoordinates();		
		currentSolution = MetropolisStep(myProblem, currentSolution, savedCoordinates, tempreture);
		solutionCost = Utillis::CalcTourDistance(bestSolution);

		if (Utillis::CalcTourDistance(currentSolution) < solutionCost)
		{
			bestSolution = currentSolution;
			solutionCost = Utillis::CalcTourDistance(bestSolution);
		}

		tempreture = UpdateTemparature(tempreture);								// updating the tempreture
	}

	Utillis::UpdateSolution(mySoulution, bestSolution, solutionCost);			// updating the solution
	std::cout << mySoulution << std::endl;
}

std::vector<std::vector<vec2>> SimulatedAnnealing::MetropolisStep(Problem & myProblem, std::vector<std::vector<vec2>> currentSolution, std::vector<vec2> savedCoordinates,float tempreture)
{
	std::vector<std::vector<vec2>> nextSolution = Utillis::getNeighbors(myProblem);
	float currentDistance = Utillis::CalcTourDistance(currentSolution);
	float nextDistance = Utillis::CalcTourDistance(nextSolution);

	if ((double)rand() / (RAND_MAX) <= calcProbability(currentDistance, nextDistance, tempreture))
	{
		currentSolution = nextSolution;
	}

	else
	{
		myProblem.setCoordinates(savedCoordinates);
	}

	return currentSolution;
}

float SimulatedAnnealing::InitTemparature()
{
	return 10000.0f;
}

float SimulatedAnnealing::UpdateTemparature(float tempreture)
{
	//float coolingFactor = 0.995;
	float coolingFactor = (float)rand() / (RAND_MAX);
	return tempreture * coolingFactor;
}

float SimulatedAnnealing::calcProbability(float currentDistance, float nextDistance, float temperature)
{
	if (nextDistance < currentDistance)									// means there is improvement then we will return the neighbor
		return 1;

	return exp((currentDistance - nextDistance) / temperature);
}

