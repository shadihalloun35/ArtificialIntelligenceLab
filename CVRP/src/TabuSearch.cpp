#include "TabuSearch.h"
#include <iostream>					// for cout etc.
#include "Utillis.h"
#include <fstream>
#define MAXSEARCHES		20000

// definition for our static variable
std::vector <std::vector<std::vector<vec2>>> TabuSearch::tabuSet;
unsigned int TabuSearch::tabuSize;

void TabuSearch::ActivateTabuSearch(Problem & myProblem)
{
	ofstream myfile;
	myfile.open("TS-problem0.txt");
	InitTabuSize();
	std::vector<std::vector<vec2>> currentSolution = Utillis::GenerateInitialSolution(myProblem);		// generating initial soulution
	std::vector<std::vector<vec2>> bestSolution = currentSolution;										// saving best soulution so far
	std::vector<std::vector<vec2>> bestCandidate = bestSolution;										// saving our best candidate
	Soulution mySoulution(bestSolution);																// creating our soulution
	mySoulution.setNumOfCarsAllowed(myProblem.getNumOfTrucks());
	float solutionCost = 0;
	InsertToTabu(bestSolution);																			// inserting to tabu list

	for (int k = 0; k < MAXSEARCHES; k++)
	{
		solutionCost = Utillis::CalcTourDistance(bestSolution);
		std::pair<std::vector<std::vector<std::vector<vec2>>>, std::vector<std::vector<vec2>>> myResult = Utillis::getNeighbors(myProblem);
		std::vector<std::vector<std::vector<vec2>>> neighbors = myResult.first;
		std::vector<std::vector<vec2>> neighborsPermutation = myResult.second;
		std::vector<std::vector<vec2>> bestCandidate = neighbors[0];
		std::vector<vec2> bestCandidateCoordinates = neighborsPermutation[0];
		
		for (size_t i = 0; i < neighbors.size(); i++)
		{

			if (Satisfiable(bestCandidate) && (Utillis::CalcTourDistance(bestCandidate) > Utillis::CalcTourDistance(neighbors[i])))
			{
				bestCandidate = neighbors[i];
				bestCandidateCoordinates = neighborsPermutation[i];
			}
		}

		if (Utillis::CalcTourDistance(bestCandidate) < solutionCost)
		{
			bestSolution = bestCandidate;
			solutionCost = Utillis::CalcTourDistance(bestSolution);
			//UpdateTabuSize();
		}

		myProblem.setCoordinates(bestCandidateCoordinates);
		InsertToTabu(bestCandidate);													// inserting to tabu list
		//UpdateTabuSize();
		myfile << "iteration " << k << ": Heuristic Value = " << solutionCost << std::endl;
	}

	myfile.close();
	Utillis::UpdateSolution(mySoulution, bestSolution, solutionCost);					// updating the solution
	std::cout << mySoulution << std::endl;
}

void TabuSearch::InsertToTabu(std::vector<std::vector<vec2>> soulotion)
{

	if (Satisfiable(soulotion))								// check if it exists first in the tabu
	{
		tabuSet.push_back(soulotion);
	}

	if (tabuSet.size() > getTabuSize())
	{
		tabuSet.erase(tabuSet.begin());						// erasing by iterator
	}
}

bool TabuSearch::Satisfiable(std::vector<std::vector<vec2>> soulotion)
{
	if (std::find(tabuSet.begin(), tabuSet.end(), soulotion) != tabuSet.end())
	{
		return false;
	}
	return true;
}


void TabuSearch::InitTabuSize()
{
	tabuSize = 10;
}

void TabuSearch::UpdateTabuSize()
{
	float factor = 1.1f;
	tabuSize = static_cast<int>(getTabuSize() * factor);
}

unsigned int TabuSearch::getTabuSize()
{
	return tabuSize;
}

