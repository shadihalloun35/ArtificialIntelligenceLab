#include "Problem.h"
#include "Init.h"



// methods we could use implementation

void Problem::Initialize(std::string fileName)
{
	Init::LoadProblem(*this,fileName);
}

bool Problem::operator==(const Problem & prob) const
{
	if (prob.capacity == capacity && prob.dimension == dimension && prob.numOfTrucks == numOfTrucks && prob.coordinates == coordinates)
		return true;
	return false;
}

// operators implementation

Problem & Problem::operator=(const Problem & prob)
{
	capacity = prob.capacity;
	dimension = prob.dimension;
	numOfTrucks = prob.numOfTrucks;
	coordinates = prob.coordinates;

	return *this;
}

// setters implementation

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




