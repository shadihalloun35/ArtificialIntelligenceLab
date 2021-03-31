#include "Tour.h"
#include "Utillis.h"


// constructor implementation

Tour::Tour()
{
}

Tour::Tour(std::vector<vec2> cities)
{
	this->cities = cities;
	this->distance = Utillis::CalcTourDistance(cities);
}


// setters implementation

void Tour::setCities(std::vector<vec2> cities)
{
	this->cities = cities;
}

void Tour::setDistance(int distance)
{
	this->distance = distance;
}

// getters implementation

std::vector<vec2> Tour::getCities()
{
	return this->cities;
}

int Tour::getDistance()
{
	return this->distance;
}

vec2 Tour::getCity(int index)
{
	return cities[index];
}
