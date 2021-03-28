#pragma once
#include <vector>
#include "vec2.h"
#include "Init.h"

class Problem
{

public:

	void Initialize(std::string fileName);					// initializing the problem


	// setters

	void setCapacity(int capacity);
	void setDimension(int dimension);
	void setCoordinates(std::vector<vec2> coordinates);
	void setDemands(std::vector<vec2> demands);


	// getters

	int getCapacity();
	int getDimension();
	std::vector<vec2> getCoordinates();
	std::vector<vec2> getDemands();

private:

	int capacity;
	int dimension;
	std::vector<vec2> coordinates;
	std::vector<vec2> demands;

};
