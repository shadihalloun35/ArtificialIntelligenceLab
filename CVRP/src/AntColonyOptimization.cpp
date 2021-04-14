#include "AntColonyOptimization.h"
#include "Utillis.h"
#include <math.h>       /* pow */
#define MAXSEARCHES		1000

/*
These are the parameters we have to use in this algorithm
*/

#define NumberOfAnts	22
#define Alpha			1.0f
#define Beta			5.0f
#define evaporation		0.5f


// definition for our static variable
std::vector <Ant> AntColonyOptimization::ants;
std::vector <Edge> AntColonyOptimization::edges;

void AntColonyOptimization::ActivateAntColonyOptimization(Problem & myProblem)
{
	InitAnts(myProblem);
	edges = myProblem.getEdges();
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
		}

		UpdatePheromone();											 // updating the pheromone
	}

	std::cout << bestSoulution << std::endl;
}

void AntColonyOptimization::InitAnts(Problem & myProblem)
{
	for (int i = 0; i < NumberOfAnts; i++) {				// generating ants
		Ant ant;
		ant.pheromone = 500; 
		ants.push_back(ant);
	}
}

void AntColonyOptimization::GenerateSolutions(Problem & myProblem, int i)
{
	Soulution soulution;
	std::vector<vec2> savedCoordinates = myProblem.getCoordinates();
	myProblem.setCoordinates(FindPermutation(myProblem,i));
	std::vector<std::vector<vec2>> antSoulution = Utillis::TrucksClassification(myProblem);	
	float solutionCost = Utillis::CalcTourDistance(antSoulution);	
	soulution.setNumOfCarsAllowed(myProblem.getNumOfTrucks());			// saving the soulution
	Utillis::UpdateSolution(soulution, antSoulution, solutionCost);
	soulution.setPermutation(myProblem.getCoordinates());
	ants[i].soulution = soulution;
	myProblem.setCoordinates(savedCoordinates);
}

std::vector<vec2> AntColonyOptimization::FindPermutation(Problem & myProblem, int i)
{
	std::vector<vec2> myPermutation;	
	std::vector<vec2> coordinates = myProblem.getCoordinates();
	myPermutation.push_back(coordinates[0]);					// storing the depot
	AddToTabu(ants[i], coordinates[0]);							// adding the depot to tabu

	for (int k = 0; k < coordinates.size() - 1; k++)
	{
		std::vector<float> probabilties = CalcProbability(myProblem, myPermutation[k], i);
		float denominator = probabilties.back();					// for probability 
		float probabilty = (rand() / static_cast<float>(RAND_MAX));
		int index = 0;
		while (probabilty -= (probabilties[i]/denominator) > 0)					// choosing the next city to visit
			++index;

		myPermutation.push_back(coordinates[index]);
		AddToTabu(ants[i], coordinates[index]);							// adding the city to tabu
		AddToList(coordinates[k], coordinates[index], i);				// adding this edge to a list
	}
}

void AntColonyOptimization::AddToList(vec2 city1, vec2 city2, int i)
{
	for (int j = 0; j < edges.size(); j++)
	{
		if (city1 == edges[j].getStart() || city1 == edges[j].getEnd())
		{
			if (city2 == edges[j].getStart() || city2 == edges[j].getEnd())	// we found the edge
			{
				Edge *edge = &edges[j];
				ants[i].edges.push_back(edge);								// pointer to the edge in the static array
				break;
			}
		}
	}

}

void AntColonyOptimization::AddToTabu(Ant & ant , vec2 city)
{
	if (std::find(ant.tabuSet.begin(), ant.tabuSet.end(), city) == ant.tabuSet.end())
	{
		ant.tabuSet.push_back(city);
	}
}

std::vector<float> AntColonyOptimization::CalcProbability(Problem & myProblem, vec2 currentCity, int i)
{
	std::vector<float> probabilties;
	float denominator = 0;				// for probability 
	for (int i = 0; i < myProblem.getCoordinates().size(); i++)
	{
		if (std::find(ants[i].tabuSet.begin(), ants[i].tabuSet.end(), currentCity) != ants[i].tabuSet.end())
		{
			probabilties.push_back(0.0f);
		}

		for (int j = 0; j < edges.size(); j++)
		{
			if (myProblem.getCoordinates()[i] == edges[j].getStart() || myProblem.getCoordinates()[i] == edges[j].getEnd())	// we found the edge
			{
				float numerator = pow(edges[j].getPheromone(), Alpha) * pow(edges[j].getVisibility(), Beta);
				denominator += numerator;
				probabilties.push_back(numerator);
			}
		}
	}
	probabilties.push_back(denominator);
}

void AntColonyOptimization::UpdatePheromone()
{
	for (size_t i = 0; i < edges.size(); i++)
	{
		float pheromone = 0;
		for (int k = 0; k < NumberOfAnts; k++)
		{
			if (std::find(ants[k].edges.begin(), ants[k].edges.end(), &edges[i]) == ants[k].edges.end())
				pheromone += ants[k].pheromone / (1/edges[i].getVisibility());

		}
		pheromone = pheromone + edges[i].getPheromone() * (1 - evaporation);
		//std::cout << soulutions[i].getPheromone() << std::endl;
		edges[i].setPheromone(pheromone);
	}
}


