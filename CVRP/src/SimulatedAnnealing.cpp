#include <iostream>					// for cout etc.
#include "SimulatedAnnealing.h"
#include "Utillis.h"
#include <algorithm>	// for random_shuffle
#include <math.h>       /* exp */
#define MAXSEARCHES		100000

void print(std::vector<std::vector<vec2>> solotion)
{
	for (int i = 0; i < solotion.size(); i++)
	{
		for (int k = 0; k < solotion[i].size(); k++)
		{
			std::cout << "(" << solotion[i][k].x << "," << solotion[i][k].y << ")-";
		}
		std::cout << std::endl;
	}
}

void SimulatedAnnealing::ActivateSimulatedAnnealing(Problem& myProblem)
{
	float tempreture = InitTemparature();
	
	std::vector<std::vector<vec2>> currentSolution = Utillis::GenerateInitialSolution(myProblem);		// generating initial solution
	std::vector<std::vector<vec2>> bestSolution = currentSolution;										// saving best solution so far


	for (int k = 0; k < MAXSEARCHES; k++)
	{
		std::vector<vec2> coordinates = myProblem.getCoordinates();
		std::vector<vec2> savedCoordinates = coordinates;
		int index1 = 0, index2 = 0;

		do {
			index1 = (int)((coordinates.size()-1) * ((double)rand() / (RAND_MAX)));					        // choosing random index to swap with
			index2 = (int)((coordinates.size()-1) * ((double)rand() / (RAND_MAX)));
		} while (index1 == 0 || index2 == 0);
		
		std::iter_swap(coordinates.begin() + index1, coordinates.begin() + index2);
		myProblem.setCoordinates(coordinates);

		std::vector<std::vector<vec2>> nextSolution = Utillis::TrucksClassification(myProblem);

		float currentDistance = Utillis::CalcTourDistance(currentSolution);
		float nextDistance = Utillis::CalcTourDistance(nextSolution);

		if ((double)rand() / (RAND_MAX) < calcProbability(currentDistance, nextDistance, tempreture)) {
			currentSolution = nextSolution;
		}
		else
			myProblem.setCoordinates(savedCoordinates);

		if (Utillis::CalcTourDistance(currentSolution) < Utillis::CalcTourDistance(bestSolution)) {
			bestSolution = currentSolution;
		}

		tempreture = UpdateTemparature(tempreture);						// updating the tempreture
	}

	print(bestSolution);

	std::cout << Utillis::CalcTourDistance(bestSolution) << std::endl;
}

float SimulatedAnnealing::InitTemparature()
{
	return 5000.0f;
}

float SimulatedAnnealing::UpdateTemparature(float tempreture)
{
	float coolingFactor = 0.995;
	//float coolingFactor = (double)rand() / (RAND_MAX);
	return tempreture * coolingFactor;
}

float SimulatedAnnealing::calcProbability(float currentDistance, float nextDistance, float temperature)
{
	if (nextDistance < currentDistance)									// means there is improvement then we will return the neighbor
		return 1;

	return exp((currentDistance - nextDistance) / temperature);
}

