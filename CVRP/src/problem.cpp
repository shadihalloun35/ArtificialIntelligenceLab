#include "problem.h"


// setters implementation

void problem::setCapacity(int capacity)
{
	this->capacity = capacity;
}

void problem::setDimension(int dimension)
{
	this->dimension = dimension;
}

void problem::setCoordinates(std::vector<vec2> coordinates)
{
	this->coordinates = coordinates;
}

void problem::setDemands(std::vector<vec2> demands)
{
	this->demands = demands;
}


// getters implementation

int problem::getCapacity()
{
	return capacity;
}

int problem::getDimension()
{
	return dimension;
}

std::vector<vec2> problem::getCoordinates()
{
	return coordinates;
}

std::vector<vec2> problem::getDemands()
{
	return demands;
}



