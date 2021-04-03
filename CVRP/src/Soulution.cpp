#include "Soulution.h"
#include "Utillis.h"


// constructor implementation

Soulution::Soulution()
{
}

Soulution::Soulution(std::vector<std::vector<vec2>> trucksTour)
{
	this->trucksTour = trucksTour;
	this->distance = 0;
}


// setters implementation

void Soulution::setTrucksTour(std::vector<std::vector<vec2>> trucksTour)
{
	this->trucksTour = trucksTour;
}

void Soulution::setDistance(float distance)
{
	this->distance = distance;
}

void Soulution::setNumOfCarsAllowed(int numOfCarsAllowed)
{
	this->numOfCarsAllowed = numOfCarsAllowed;
}

// getters implementation

std::vector<std::vector<vec2>>& Soulution::getTrucksTour()
{
	return this->trucksTour;
}

float Soulution::getDistance()
{
	return this->distance;
}

int Soulution::getNumOfCarsAllowed()
{
	return this->numOfCarsAllowed;
}

std::vector<vec2> Soulution::getTruckTour(int index)
{
	return trucksTour[index];
}

// operators implementation

Soulution & Soulution::operator=(const Soulution & sol)
{
	trucksTour = sol.trucksTour;
	distance = sol.distance;
	numOfCarsAllowed = sol.numOfCarsAllowed;

	return *this;
}


ostream & operator << (ostream &out, Soulution &solution)
{
	int zero = 0;
	out << solution.getDistance() << " " << zero << std::endl;
	Utillis::PrintSolution(solution);
	return out;
}