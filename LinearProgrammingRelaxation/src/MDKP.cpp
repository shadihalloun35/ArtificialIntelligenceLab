#include "MDKP.h"
#include "Init.h"



// methods we could use implementation

void MDKP::InitializeMDKP(std::string fileName)
{
	Init::LoadMDKP(*this, fileName);
}



// operators implementation


MDKP & MDKP::operator=(const MDKP & prob)
{
	numOfKnapsacks = prob.numOfKnapsacks;
	numOfObjects = prob.numOfObjects;
	knapsacks = prob.knapsacks;

	return *this;
}

/**
bool MDKP::operator==(const MDKP & prob) const
{
	if (prob.numOfKnapsacks == numOfKnapsacks && prob.numOfObjects == numOfObjects && prob.knapsacks == knapsacks)
		return true;
	return false;
}
*/

// setters implementation

void MDKP::setNumOfKnapsacks(int numOfKnapsacks)
{
	this->numOfKnapsacks = numOfKnapsacks;
}

void MDKP::setNumOfObjects(int numOfObjects)
{
	this->numOfObjects = numOfObjects;
}

void MDKP::setKnapsacks(std::vector<knapsack> knapsacks)
{
	this->knapsacks = knapsacks;
}

// getters implementation

int MDKP::getNumOfKnapsacks()
{
	return numOfKnapsacks;
}

int MDKP::getNumOfObjects()
{
	return numOfObjects;
}


std::vector<knapsack> MDKP::getKnapsacks()
{
	return knapsacks;
}


