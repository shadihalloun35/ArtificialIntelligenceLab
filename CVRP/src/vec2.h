#pragma once

class vec2 {

private:
	int x;
	int y;

public:

	// methods we could use

	vec2();											// constructors						
	vec2(int x, int y);

	vec2 operator+(vec2& v);						// arithmetic methods
	vec2 operator-(vec2& v);
	vec2& operator+=(vec2& v);
	vec2& operator-=(vec2& v);

	float distance(vec2 v) const;				  // distance between two points
	

};