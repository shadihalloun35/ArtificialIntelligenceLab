#pragma once
#include "Problem.h"
#include "Tour.h"

class SimulatedAnnealing
{
public:
	static void ActivateSimulatedAnnealing(Problem& myProblem);
	static float InitTemparature();
	static float UpdateTemparature(float tempreture);
	static float calcProbability(float currentDistance, float nextDistance, float temperature);
};




