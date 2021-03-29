#include "Problem.h"
#include "Init.h"


// setters implementation


void Problem::Initialize(std::string fileName)
{
	Init::LoadProblem(*this,fileName);
}

void Problem::setCapacity(int capacity)
{
	this->capacity = capacity;
}

void Problem::setDimension(int dimension)
{
	this->dimension = dimension;
}

void Problem::setNumOfTrucks(int numOfTrucks)
{
	this->numOfTrucks = numOfTrucks;
}

void Problem::setCoordinates(std::vector<vec2> coordinates)
{
	this->coordinates = coordinates;
}

void Problem::setDemands(std::vector<int> demands)
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

int Problem::getNumOfTrucks()
{
	return numOfTrucks;
}

std::vector<vec2>& Problem::getCoordinates()
{
	return coordinates;
}

std::vector<int>& Problem::getDemands()
{
	return demands;
}



