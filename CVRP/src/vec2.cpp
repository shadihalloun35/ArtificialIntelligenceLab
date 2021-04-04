#include "vec2.h"
#include <cmath>				// for sqrt function



vec2::vec2() :x(0), y(0) {}
vec2::vec2(int x, int y) : x(x), y(y) {}

// operators implementation

vec2 vec2::operator+(vec2 & v)
{
	return vec2(x + v.x, y + v.y);
}

vec2 vec2::operator-(vec2& v) {
	return vec2(x - v.x, y - v.y);
}

vec2 & vec2::operator+=(vec2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

vec2 & vec2::operator-=(vec2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

bool vec2::operator==(const vec2 & v) const
{
	if(v.x == x && v.y == y && v.index == index && v.demand == demand)
		return true;
	return false;
}


vec2& vec2::operator=(const vec2& v) {
	x = v.x;
	y = v.y;
	index = v.index;
	demand = v.demand;

	return *this;
}


// setters implementation

void vec2::setDemand(int demand)
{
	this->demand = demand;
}

void vec2::setIndex(int index)
{
	this->index = index;
}

// getters implementation

int vec2::getDemand()
{
	return this->demand;
}

int vec2::getIndex()
{
	return this->index;
}

// methods we could use implementation

float vec2::distance(vec2 v) const
{
	int diffX = v.x - x;
	int diffY = v.y - y;

	return (float)std::sqrt(diffX * diffX + diffY * diffY);
}
