#include "vec2.h"
#include <cmath>				// for sqrt function



vec2::vec2() :x(0), y(0) {}
vec2::vec2(int x, int y) : x(x), y(y) {}

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

float vec2::distance(vec2 v) const
{
	float diffX = v.x - x;
	float diffY = v.y - y;

	return std::sqrt(diffX * diffX + diffY * diffY);
}
