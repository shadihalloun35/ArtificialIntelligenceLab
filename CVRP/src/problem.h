#pragma once
#include <vector>
#include "vec2.h"

class Problem
{

public:

	// methods we could use
	void Initialize(std::string fileName);					// initializing the problem

	// operators
	Problem& operator=(const Problem& prob);

	// setters

	void setCapacity(int capacity);
	void setDimension(int dimension);
	void setNumOfTrucks(int dimension);
	void setCoordinates(std::vector<vec2> coordinates);


	// getters

	int getCapacity();
	int getDimension();
	int getNumOfTrucks();
	std::vector<vec2>& getCoordinates();

private:

	int capacity;
	int dimension;
	int numOfTrucks;
	std::vector<vec2> coordinates;

};
