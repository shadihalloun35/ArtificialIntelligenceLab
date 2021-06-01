#include "Edge.h"

Edge::Edge()
{
	this->start = vec2(0, 0);
	this->end = vec2(0, 0);
	this->pheromone = 1.0f;
	this->visibility = 1 / start.distance(end);
}

Edge::Edge(vec2 start, vec2 end)
{
	this->start = start;
	this->end = end;
	this->pheromone = 1.0f;
	this->visibility = 1 / start.distance(end);
}

Edge & Edge::operator=(const Edge & v)
{
	this->start = v.start;
	this->end = v.end;

	return *this;
}

bool Edge::operator==(const Edge & v) const
{
	if (this->start == v.start && this->end == v.end)
		return true;

	if (this->start == v.end && this->end == v.start)
		return true;

	return false;
}

void Edge::setStart(vec2 start)
{
	this->start = start;

}

void Edge::setEnd(vec2 end)
{
	this->end = end;
}

void Edge::setPheromone(float pheromone)
{
	this->pheromone = pheromone;
}

void Edge::setVisibility(float visibility)
{
	this->visibility = visibility;
}

vec2 Edge::getStart()
{
	return this->start;
}

vec2 Edge::getEnd()
{
	return this->end;
}

float & Edge::getPheromone()
{
	return this->pheromone;
}

float Edge::getVisibility()
{
	return this->visibility;
}
