#pragma once

class vec2 {


public:

	int x;
	int y;
	int demand;
	int index;

	// constructors
	vec2();
	vec2(int x, int y);

	// operators
	vec2 operator+(vec2& v);
	vec2 operator-(vec2& v);
	vec2& operator+=(vec2& v);
	vec2& operator-=(vec2& v);
	vec2& operator=(const vec2& v);
	bool operator ==(const vec2 &v) const;


	// setters
	void setDemand(int demand);
	void setIndex(int index);


	// getters 
	int getDemand();
	int getIndex();

	// methods we could use
	float distance(vec2 v) const;				  // distance between two points


};