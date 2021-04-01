#include <iostream>					// for cout etc.
#include "Utillis.h"
#include "Tour.h"
#include <algorithm>				// for random_shuffle


float Utillis::CalcTourDistance(std::vector<std::vector<vec2>> trucksTour)
{
	float totalDistance = 0;

	for (int i = 0; i < trucksTour.size(); i++)
	{
		std::vector<vec2> currentTruckTour = trucksTour[i];
		int numOfCities = currentTruckTour.size();

		for (int i = 0; i < numOfCities; i++)
		{
			vec2 start = currentTruckTour[i];
			vec2 end = currentTruckTour[i + 1 < numOfCities ? i + 1 : 0];
			totalDistance += start.distance(end);
		}
	}
	
	return totalDistance;
}

std::vector<std::vector<vec2>> Utillis::GenerateInitialSolution(Problem &myProblem)
{
	Tour currentTour(myProblem.getCoordinates());
	std::random_shuffle(currentTour.getCities().begin(), currentTour.getCities().end());			// shuffling the coordinates
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

