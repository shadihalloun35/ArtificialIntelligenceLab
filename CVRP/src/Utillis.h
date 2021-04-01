#pragma once
#include "vec2.h"
#include <vector>
#include "Problem.h"


class Utillis
{
public:
	static float CalcTourDistance(std::vector<std::vector<vec2>> trcukTours);
	static std::vector<std::vector<vec2>> GenerateInitialSolution(Problem& myProblem);
	static std::vector<std::vector<vec2>> TrucksClassification(Problem & myProblem);

};