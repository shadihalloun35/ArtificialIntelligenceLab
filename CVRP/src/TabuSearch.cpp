#include "TabuSearch.h"
#include "Utillis.h"
#define MAXSEARCHES		1000000

// definition for our static variable
unordered_set <std::vector<std::vector<vec2>>> TabuSearch::tabuSet;
int TabuSearch::tabuSize;

void TabuSearch::ActivateTabuSearch(Problem & myProblem)
{
	InitTabuSize();
	std::vector<std::vector<vec2>> currentSolution = Utillis::GenerateInitialSolution(myProblem);		// generating initial solution
	std::vector<std::vector<vec2>> bestSolution = currentSolution;										// saving best solution so far
	std::vector<std::vector<vec2>> bestCandidate = bestSolution;										// saving our best candidate
	Soulution mySoulution(bestSolution);																// creating our solution
	mySoulution.setNumOfCarsAllowed(myProblem.getNumOfTrucks());
	float solutionCost = 0;
	InsertToTabu(bestSolution);								// inserting to tabu list

	for (int k = 0; k < MAXSEARCHES; k++)
	{
		std::vector<vec2> savedCoordinates = myProblem.getCoordinates();
		std::vector<std::vector<vec2>> nextSolution = Utillis::getNeighbors(myProblem);
		bestCandidate = nextSolution;
		if (Satisfiable(nextSolution) && (Utillis::CalcTourDistance(bestCandidate) > Utillis::CalcTourDistance(nextSolution)))
		{
			bestCandidate = nextSolution;
		}
		solutionCost = Utillis::CalcTourDistance(bestSolution);

		if (Utillis::CalcTourDistance(bestCandidate) < solutionCost)
		{
			bestSolution = currentSolution;
			solutionCost = Utillis::CalcTourDistance(bestSolution);
		}
		else
		{
			myProblem.setCoordinates(savedCoordinates);
		}
		InsertToTabu(bestCandidate);							// inserting to tabu list
		UpdateTabuSize();
	}


}

void TabuSearch::InsertToTabu(std::vector<std::vector<vec2>> soulotion)
{
	if (tabuSet.find(soulotion) == tabuSet.end())			// check if it exists first in the tabu
	{
		tabuSet.insert(soulotion);
	}

	if (tabuSet.size() > getTabuSize())
	{
		tabuSet.erase(tabuSet.begin());									// erasing by iterator
	}
}

bool TabuSearch::Satisfiable(std::vector<std::vector<vec2>> soulotion)
{
	if (tabuSet.find(soulotion) == tabuSet.end())			// return true if it's not in the tabu
	{
		return true;
	}
	return false;
}


void TabuSearch::InitTabuSize()
{
	tabuSize = 20;
}

void TabuSearch::UpdateTabuSize()
{
	float factor = 1.5;
	tabuSize = static_cast<int>(getTabuSize() * factor);
}

int TabuSearch::getTabuSize()
{
	return 0;
}
