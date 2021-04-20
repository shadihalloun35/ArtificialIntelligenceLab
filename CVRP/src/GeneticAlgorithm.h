#pragma once
#include "Problem.h"
#include "Soulution.h"


struct ga_struct
{
	std::vector<vec2> permutation;	   // the permutation of the whole tour
	Soulution soulution;			   // the trucks tour
	float fitness;					   // its fitness;
	unsigned int age;
};

class GeneticAlgorithm
{

public:
	static void ActivateGeneticAlgorithm(Problem & myProblem);
};

