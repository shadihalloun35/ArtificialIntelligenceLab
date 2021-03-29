#pragma once
#include <vector>
#include "vec2.h"

class Problem
{

public:

	void Initialize(std::string fileName);					// initializing the problem

	// setters

	void setCapacity(int capacity);
	void setDimension(int dimension);
	void setNumOfTrucks(int dimension);
	void setCoordinates(std::vector<vec2> coordinates);
	void setDemands(std::vector<int> demands);


	// getters

	int getCapacity();
	int getDimension();
	int getNumOfTrucks();
	std::vector<vec2>& getCoordinates();
	std::vector<int>& getDemands();

private:

	int capacity;
	int dimension;
	int numOfTrucks;
	std::vector<vec2> coordinates;
	std::vector<int> demands;

};
