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

float vec2::distance(vec2 v) const
{
	float diffX = v.x - x;
	float diffY = v.y - y;

	return std::sqrt(diffX * diffX + diffY * diffY);
}
