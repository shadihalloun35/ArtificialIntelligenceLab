#include <iostream>					// for cout etc.
#include "Utillis.h"
#include <algorithm>				// for random_shuffle


float Utillis::CalcTourDistance(std::vector<std::vector<vec2>> trucksTour)
{
	float totalDistance = 0;

	for (int i = 0; i < trucksTour.size(); i++)
	{
		std::vector<vec2> currentTruckTour = trucksTour[i];
		int numOfCities = currentTruckTour.size();

		for (int j = 0; j < numOfCities; j++)
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

void Utillis::UpdateSolution(Soulution & mySoulution, std::vector<std::vector<vec2>> bestSolution, float solutionCost)
{
	mySoulution.setTrucksTour(bestSolution);
	mySoulution.setDistance(solutionCost);
}

void Utillis::PrintSolution(Soulution & mySoulution)
{
	for (int i = 0; i < mySoulution.getTrucksTour().size(); i++)								// printing the path of every truck
	{
		for (int k = 0; k < mySoulution.getTrucksTour()[i].size(); k++)
		{
			std::cout << mySoulution.getTrucksTour()[i][k].getIndex() << " " ;
		}
		std::cout << mySoulution.getTrucksTour()[0][0].getIndex() << std::endl;
	}

	for (int i = 0; i < mySoulution.getNumOfCarsAllowed() - mySoulution.getTrucksTour().size(); i++) // printing the path of the rest 
	{																								 // of the rest of the cars
		std::cout << mySoulution.getTrucksTour()[0][0].getIndex() << " ";
		std::cout << mySoulution.getTrucksTour()[0][0].getIndex() << std::endl;
	}

}

