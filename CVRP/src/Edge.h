#pragma once
#include "vec2.h"

class Edge {

private:
	vec2 start;
	vec2 end;
	float pheromone;
	float visibility;

public:

	// constructors
	Edge();
	Edge(vec2 start, vec2 end);

	// operators
	Edge& operator=(const Edge& v);
	bool operator ==(const Edge &v) const;


	// setters
	void setStart(vec2 start);
	void setEnd(vec2 end);
	void setPheromone(float pheromone);
	void setVisibility(float visibility);


	// getters 
	vec2 getStart();
	vec2 getEnd();
	float& getPheromone();
	float getVisibility();
};
