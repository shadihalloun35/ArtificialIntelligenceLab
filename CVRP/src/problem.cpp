#include "Problem.h"


// setters implementation


void Problem::Initialize(std::string fileName)
{
	Init::init_problem(fileName);

}

void Problem::setCapacity(int capacity)
{
	this->capacity = capacity;
}

void Problem::setDimension(int dimension)
{
	this->dimension = dimension;
}

void Problem::setCoordinates(std::vector<vec2> coordinates)
{
	this->coordinates = coordinates;
}

void Problem::setDemands(std::vector<vec2> demands)
{
	this->demands = demands;
}


// getters implementation

int Problem::getCapacity()
{
	return capacity;
}

int Problem::getDimension()
{
	return dimension;
}

std::vector<vec2> Problem::getCoordinates()
{
	return coordinates;
}

std::vector<vec2> Problem::getDemands()
{
	return demands;
}



