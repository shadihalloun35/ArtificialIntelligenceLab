#include "SimulatedAnnealing.h"
#include "Utillis.h"
#include <algorithm>	// for random_shuffle
#define MAXSEARCHES		1000

void SimulatedAnnealing::ActivateSimulatedAnnealing(Problem& myProblem)
{
	float tempreture = InitTemparature();
	std::vector<std::vector<vec2>> currentSolution = Utillis::GenerateInitialSolution(myProblem);		// generating initial solution
	std::vector<std::vector<vec2>> bestSolution = currentSolution;										// generating initial solution

	for (int k = 0; k < MAXSEARCHES; k++)
	{

		std::vector<std::vector<vec2>> neighbor = currentSolution;

		int index1 = (int)(neighbor.noCities() * Math.random());
		int index2 = (int)(neighbor.noCities() * Math.random());

		Collections.swap(next.getCities(), index1, index2);

		int currentLength = current.getTourLength();
		int neighborLength = neighbor.getTourLength();

		if (Math.random() < Util.probability(currentLength, neighborLength, t)) {
			current = neighbor.duplicate();
		}

		if (current.getTourLength() < best.getTourLength()) {
			best = current.duplicate();
		}

	}


}

float SimulatedAnnealing::InitTemparature()
{
	return 1000.0f;
}
