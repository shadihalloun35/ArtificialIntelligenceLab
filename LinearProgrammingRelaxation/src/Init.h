#pragma once
#include "CVRP.h"
#include "MDKP.h"
#include <iostream>
#include <fstream>
#include <string>



class Init
{
public:
	static void LoadCVRP(CVRP &myCVRP, std::string fileName);
	static void LoadMDKP(MDKP &myMDKP, std::string fileName);
	static int FindDimension(std::string line);
	static int FindCapacity(std::string line);
	static vec2 FindCoordinates(std::string line);
	static int FindDemands(std::string line);
	static void GenerateEdges(CVRP & myCVRP);
};