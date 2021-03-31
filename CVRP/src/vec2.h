#pragma once

class vec2 {


public:

	int x;
	int y;
	int demand;

	// methods we could use

	vec2();											// constructors						
	vec2(int x, int y);

	vec2 operator+(vec2& v);						// arithmetic methods
	vec2 operator-(vec2& v);
	vec2& operator+=(vec2& v);
	vec2& operator-=(vec2& v);

	// setters
	void setDemand(int demand);

	// getters 
	int getDemand();

	float distance(vec2 v) const;				  // distance between two points
	

};