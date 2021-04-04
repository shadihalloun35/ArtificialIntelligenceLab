#pragma once
#include "Problem.h"
#include "Soulution.h"

class SimulatedAnnealing
{

public:
	static void ActivateSimulatedAnnealing(Problem& myProblem);
	static std::vector<std::vector<vec2>> MetropolisStep(Problem& myProblem, std::vector<std::vector<vec2>> currentSolution, std::vector<vec2> savedCoordinates, float tempreture);
	static float calcProbability(float currentDistance, float nextDistance, float temperature);
	static void InitTemparature();
	static void UpdateTemparature();
	static float getTemparature();


private:
	static float temparature;
};




