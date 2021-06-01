#include "CVRP.h"
#include "Init.h"



// methods we could use implementation

void CVRP::Initialize(std::string fileName)
{
	Init::LoadCVRP(*this, fileName);
}

bool CVRP::operator==(const CVRP & prob) const
{
	if (prob.capacity == capacity && prob.dimension == dimension && prob.numOfTrucks == numOfTrucks && prob.coordinates == coordinates)
		return true;
	return false;
}

// operators implementation

CVRP & CVRP::operator=(const CVRP & prob)
{
	capacity = prob.capacity;
	dimension = prob.dimension;
	numOfTrucks = prob.numOfTrucks;
	coordinates = prob.coordinates;

	return *this;
}

// setters implementation

void CVRP::setCapacity(int capacity)
{
	this->capacity = capacity;
}

void CVRP::setDimension(int dimension)
{
	this->dimension = dimension;
}

void CVRP::setNumOfTrucks(int numOfTrucks)
{
	this->numOfTrucks = numOfTrucks;
}

void CVRP::setCoordinates(std::vector<vec2> coordinates)
{
	this->coordinates = coordinates;
}

void CVRP::setEdges(std::vector<Edge> edges)
{
	this->edges = edges;
}

// getters implementation

int CVRP::getCapacity()
{
	return capacity;
}

int CVRP::getDimension()
{
	return dimension;
}

int CVRP::getNumOfTrucks()
{
	return numOfTrucks;
}

std::vector<vec2>& CVRP::getCoordinates()
{
	return coordinates;
}

std::vector<Edge>& CVRP::getEdges()
{
	return this->edges;
}




