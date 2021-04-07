#pragma once
#include "vec2.h"
#include <vector>
#include "Problem.h"
#include "Soulution.h"


class Utillis
{
public:
	static float CalcTourDistance(std::vector<std::vector<vec2>> trucksTour);
	static std::vector<std::vector<vec2>> GenerateInitialSolution(Problem& myProblem);
	static std::vector<std::vector<vec2>> TrucksClassification(Problem & myProblem);
	static std::vector<std::vector<vec2>> getNeighbor(Problem & myProblem);
	static std::pair<std::vector<std::vector<std::vector<vec2>>>, std::vector<std::vector<vec2>>> getNeighbors(Problem & myProblem);
	static void UpdateSolution(Soulution & mySoulution, std::vector<std::vector<vec2>> bestSolution, float solutionCost);
	static void PrintSolution(Soulution & mySoulution);


};