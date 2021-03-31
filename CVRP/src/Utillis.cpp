#include "Utillis.h"
#include "Tour.h"
#include <algorithm>						// for random_shuffle


float Utillis::CalcTourDistance(std::vector<vec2> cities)
{

	int numOfCities = cities.size();
	float totalDistance;

	for (int i = 0 ; i < numOfCities; i++)
	{
		vec2 start = cities[i];
		vec2 end = cities[i + 1 < numOfCities ? i + 1 : 0];
		totalDistance += start.distance(end);
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
	std::vector<std::vector<vec2>> allTrucksTours;
	int capacity = myProblem.getCapacity();
	int numOfTrucksUsed = 0;
	std::vector<vec2> coordinates = myProblem.getCoordinates();
	std::vector<vec2> savedCoordinates = coordinates;

	while (coordinates.size() != 1) {

		std::vector<vec2> truckTour;

		while (capacity > coordinates[1].getDemand())
		{
			capacity -= coordinates[1].getDemand();
			truckTour.push_back(coordinates[1]);
			coordinates.erase(coordinates.begin() + 1);
		}

		allTrucksTours.push_back(truckTour);
		numOfTrucksUsed += 1;

		if (numOfTrucksUsed > myProblem.getNumOfTrucks())
			break;
	}
}


