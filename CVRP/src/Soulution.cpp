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

void Soulution::setPermutation(std::vector<vec2> permutation)
{
	this->permutation = permutation;
}

void Soulution::setDistance(float distance)
{
	this->distance = distance;
}

void Soulution::setNumOfCarsAllowed(int numOfCarsAllowed)
{
	this->numOfCarsAllowed = numOfCarsAllowed;
}

void Soulution::setPheromone(float pheromone)
{
	this->pheromone = pheromone;
}

void Soulution::setVisibility(float visibility)
{
	this->visibility = visibility;
}

// getters implementation

std::vector<std::vector<vec2>>& Soulution::getTrucksTour()
{
	return this->trucksTour;
}

std::vector<vec2>& Soulution::getPermutation()
{
	return this->permutation;
}

float Soulution::getDistance()
{
	return this->distance;
}

int Soulution::getNumOfCarsAllowed()
{
	return this->numOfCarsAllowed;
}

float & Soulution::getPheromone()
{
	return this->pheromone;

}

float Soulution::getVisibility()
{
	return this->visibility;
}

std::vector<vec2> Soulution::getTruckTour(int index)
{
	return trucksTour[index];
}

// operators implementation

Soulution & Soulution::operator=(const Soulution & sol)
{
	trucksTour = sol.trucksTour;
	permutation = sol.permutation;
	distance = sol.distance;
	numOfCarsAllowed = sol.numOfCarsAllowed;
	pheromone = sol.pheromone;
	visibility = sol.visibility;

	return *this;
}

bool Soulution::operator==(const Soulution & sol) const
{
	if (sol.trucksTour == trucksTour)
		return true;
	return false;
}


ostream & operator << (ostream &out, Soulution &solution)
{
	int zero = 0;
	out << solution.getDistance() << " " << zero << std::endl;
	Utillis::PrintSolution(solution);
	return out;
}