#pragma once
#pragma once
#include <vector>
#include "vec2.h"


class Tour
{

public:

	// constructors

	Tour();
	Tour(std::vector<vec2> cities);

	// setters

	void setCities(std::vector<vec2> cities);
	void setDistance(int distance);


	// getters

	std::vector<vec2>& getCities();
	int getDistance();
	vec2 getCity(int index);
	

private:

	std::vector<vec2> cities;
	float distance;


};
