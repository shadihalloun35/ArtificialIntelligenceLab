#pragma once
#include <vector>

struct knapsack
{
	std::vector<int> values;
	std::vector<int> weights;
	int capacity;
};

class MDKP
{

public:

	// methods we could use
	void Initialize(std::string fileName);					// initializing the problem

	// operators
	MDKP& operator =(const MDKP & prob);
	bool operator ==(const MDKP & prob) const;

	// setters

	void setNumOfKnapsacks(int numOfKnapsacks);
	void setNumOfObjects(int numOfObjects);
	void setKnapsacks(std::vector<knapsack> knapsacks);


	// getters

	int getNumOfKnapsacks();
	int getNumOfObjects();
	std::vector<knapsack> getKnapsacks();


private:

	int numOfKnapsacks;
	int numOfObjects;
	std::vector<knapsack> knapsacks;
};
