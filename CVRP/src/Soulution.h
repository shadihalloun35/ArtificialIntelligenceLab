#pragma once
#pragma once
#include <vector>
#include "vec2.h"
#include <iostream>					// for cout etc.
using namespace std;

class Soulution
{

public:

	// constructors

	Soulution();
	Soulution(std::vector<std::vector<vec2>> trucksTour);

	// setters

	void setTrucksTour(std::vector<std::vector<vec2>> trucksTour);
	void setDistance(float distance);
	void setNumOfCarsAllowed(int numOfCarsAllowed);

	// getters

	std::vector<std::vector<vec2>>& getTrucksTour();
	float getDistance();
	int getNumOfCarsAllowed();
	std::vector<vec2> getTruckTour(int index);


	// operators 
	Soulution& operator=(const Soulution& sol);
	friend ostream & operator << (ostream &out, Soulution &solution);



private:

	std::vector<std::vector<vec2>> trucksTour;
	float distance;
	int numOfCarsAllowed;

};
