#pragma once
#include "problem.h"
#include <iostream>
#include <fstream>
#include <string>



class Init
{
public:
	static void LoadProblem(Problem &myProblem, std::string fileName);
	static int FindDimension(std::string line);
	static int FindCapacity(std::string line);
	static vec2 FindCoordinates(std::string line);
	static int FindDemands(std::string line);
	static void GenerateEdges(Problem & myProblem);

};