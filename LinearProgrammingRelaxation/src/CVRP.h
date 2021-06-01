#pragma once
#include <vector>
#include "vec2.h"
#include "Edge.h"

class CVRP
{

public:

	// methods we could use
	void Initialize(std::string fileName);					// initializing the problem

	// operators
	CVRP& operator =(const CVRP& prob);
	bool operator ==(const CVRP &prob) const;

	// setters

	void setCapacity(int capacity);
	void setDimension(int dimension);
	void setNumOfTrucks(int dimension);
	void setCoordinates(std::vector<vec2> coordinates);
	void setEdges(std::vector<Edge> edges);



	// getters

	int getCapacity();
	int getDimension();
	int getNumOfTrucks();
	std::vector<vec2>& getCoordinates();
	std::vector<Edge>& getEdges();


private:

	int capacity;
	int dimension;
	int numOfTrucks;
	std::vector<vec2> coordinates;
	std::vector<Edge> edges;
};
