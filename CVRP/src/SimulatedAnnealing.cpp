#include <iostream>					// for cout etc.
#include "SimulatedAnnealing.h"
#include "Utillis.h"
#include <algorithm>	// for random_shuffle
#include <math.h>       /* exp */
#include <fstream>
#define MAXSEARCHES		1000000

// definition for our static variable
float SimulatedAnnealing::temparature;

void SimulatedAnnealing::ActivateSimulatedAnnealing(Problem& myProblem)
{
	ofstream myfile;
	myfile.open("SA-problem0.txt");
	InitTemparature();
	std::vector<std::vector<vec2>> currentSolution = Utillis::GenerateInitialSolution(myProblem);		// generating initial soulution
	std::vector<std::vector<vec2>> bestSolution = currentSolution;										// saving best soulution so far
	Soulution mySoulution(bestSolution);																// creating our soulution
	mySoulution.setNumOfCarsAllowed(myProblem.getNumOfTrucks());
	float solutionCost = 0;

	for (int k = 0; k < MAXSEARCHES; k++)
	{
		for (int j = 0; j < getTemparature() + 1; j++)
		{
			std::vector<vec2> savedCoordinates = myProblem.getCoordinates();
			currentSolution = MetropolisStep(myProblem, currentSolution, savedCoordinates, getTemparature());
			solutionCost = Utillis::CalcTourDistance(bestSolution);

			if (Utillis::CalcTourDistance(currentSolution) < solutionCost)
			{
				bestSolution = currentSolution;
				solutionCost = Utillis::CalcTourDistance(bestSolution);
			}
		}

		UpdateTemparature();													// updating the tempreture
		myfile << "iteration " << k << ": Heuristic Value = " << solutionCost << std::endl;
	}
	myfile.close();
	Utillis::UpdateSolution(mySoulution, bestSolution, solutionCost);			// updating the solution
	std::cout << mySoulution << std::endl;
}

std::vector<std::vector<vec2>> SimulatedAnnealing::MetropolisStep(Problem & myProblem, std::vector<std::vector<vec2>> currentSolution, std::vector<vec2> savedCoordinates, float tempreture)
{
	std::vector<std::vector<vec2>> nextSolution = Utillis::getNeighbor(myProblem);
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

void SimulatedAnnealing::InitTemparature()
{
	temparature = 100.0f;
}

void SimulatedAnnealing::UpdateTemparature()
{
	float coolingFactor = 0.995f;
	//float coolingFactor = (float)rand() / (RAND_MAX);
	temparature = getTemparature() * coolingFactor;
}

float SimulatedAnnealing::getTemparature()
{
	return 0;
}

float SimulatedAnnealing::calcProbability(float currentDistance, float nextDistance, float temperature)
{
	if (nextDistance < currentDistance)									// means there is improvement then we will return the neighbor
		return 1;

	return exp((currentDistance - nextDistance) / temperature);
}

