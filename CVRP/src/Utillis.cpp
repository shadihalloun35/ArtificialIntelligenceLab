#include <iostream>					// for cout etc.
#include "Utillis.h"
#include <algorithm>				// for random_shuffle


float Utillis::CalcTourDistance(std::vector<std::vector<vec2>> trucksTour)
{
	float totalDistance = 0;

	for (size_t i = 0; i < trucksTour.size(); i++)
	{
		std::vector<vec2> currentTruckTour = trucksTour[i];
		size_t numOfCities = currentTruckTour.size();

		for (size_t j = 0; j < numOfCities; j++)
		{
			vec2 start = currentTruckTour[j];
			vec2 end = currentTruckTour[j + 1 < numOfCities ? j + 1 : 0];
			totalDistance += start.distance(end);
		}
	}

	return totalDistance;
}

std::vector<std::vector<vec2>> Utillis::GenerateInitialSolution(Problem &myProblem)
{
	std::vector<std::vector<vec2>> allTrucksTours = TrucksClassification(myProblem);				// generating random solution
	return allTrucksTours;
}

std::vector<std::vector<vec2>> Utillis::GenerateRandomSolution(Problem & myProblem)
{
	std::random_shuffle(myProblem.getCoordinates().begin()+1, myProblem.getCoordinates().end());
	std::vector<std::vector<vec2>> allTrucksTours = TrucksClassification(myProblem);				// generating random solution
	return allTrucksTours;
}

std::vector<std::vector<vec2>> Utillis::TrucksClassification(Problem &myProblem)
{
	std::vector<std::vector<vec2>> allTrucksTour;
	int numOfTrucksUsed = 0;
	std::vector<vec2> coordinates = myProblem.getCoordinates();
	std::vector<vec2> savedCoordinates = coordinates;

	while (coordinates.size() != 1) {
		int capacity = myProblem.getCapacity();
		std::vector<vec2> truckTour;
		truckTour.push_back(coordinates[0]);

		while (capacity > coordinates[1].getDemand())					// push cities to the truck tour till capacity is zero
		{
			capacity -= coordinates[1].getDemand();
			truckTour.push_back(coordinates[1]);
			coordinates.erase(coordinates.begin() + 1);

			if (coordinates.size() == 1)								// to avoid index to be out of range
				break;

		}

		allTrucksTour.push_back(truckTour);
		numOfTrucksUsed += 1;

		if (numOfTrucksUsed > myProblem.getNumOfTrucks())
			break;
	}

	myProblem.setCoordinates(savedCoordinates);
	return allTrucksTour;
}

std::vector<std::vector<vec2>> Utillis::getNeighbor(Problem & myProblem)
{
	std::vector<vec2> coordinates = myProblem.getCoordinates();
	int index1 = 0, index2 = 0;

	do {
		index1 = (int)((coordinates.size() - 1) * ((double)rand() / (RAND_MAX)));		// choosing a neighbor
		index2 = (int)((coordinates.size() - 1) * ((double)rand() / (RAND_MAX)));
	} while (index1 == 0 || index2 == 0);

	std::iter_swap(coordinates.begin() + index1, coordinates.begin() + index2);
	myProblem.setCoordinates(coordinates);
	return TrucksClassification(myProblem);
}

std::pair<std::vector<std::vector<std::vector<vec2>>>, std::vector<std::vector<vec2>>> Utillis::getNeighbors(Problem & myProblem)
{
	std::vector<std::vector<std::vector<vec2>>> neighbors;
	std::vector<std::vector<vec2>> neighborsPermutation;
	std::vector<vec2> savedCoordinates = myProblem.getCoordinates();


	for (int i = 0; i < 30; i++)
	{
		neighbors.push_back(getNeighbor(myProblem));
		neighborsPermutation.push_back(myProblem.getCoordinates());
		myProblem.setCoordinates(savedCoordinates);
	}

	return std::make_pair(neighbors, neighborsPermutation);
}

void Utillis::UpdateSolution(Soulution & mySoulution, std::vector<std::vector<vec2>> bestSolution, float solutionCost)
{
	mySoulution.setTrucksTour(bestSolution);
	mySoulution.setDistance(solutionCost);
	mySoulution.setVisibility(1/solutionCost);

}

void Utillis::PrintSolution(Soulution & mySoulution)
{
	for (size_t i = 0; i < mySoulution.getTrucksTour().size(); i++)								// printing the path of every truck
	{
		for (size_t k = 0; k < mySoulution.getTrucksTour()[i].size(); k++)
		{
			std::cout << mySoulution.getTrucksTour()[i][k].getIndex() << " ";
		}
		std::cout << mySoulution.getTrucksTour()[0][0].getIndex() << std::endl;
	}

	for (size_t i = 0; i < mySoulution.getNumOfCarsAllowed() - mySoulution.getTrucksTour().size(); i++) // printing the path of the rest 
	{																								 // of the rest of the cars
		std::cout << mySoulution.getTrucksTour()[0][0].getIndex() << " ";
		std::cout << mySoulution.getTrucksTour()[0][0].getIndex() << std::endl;
	}

}

