#include "Init.h"
#include <string> 
#include <algorithm>    // For std::remove()

#define NumOfTrucks		20

int coordinateIndex = 0;

void Init::LoadCVRP(CVRP &myCVRP, std::string fileName)
{
	int lineNumber = 0, dimension = 0, index = 0;
	std::string line;
	std::ifstream fin;
	std::string filePath("C:\\ArtificialIntelligenceLab\\ArtificialIntelligenceLab\\CVRP\\problems\\");
	fin.open(filePath + fileName, std::ios::in);

	while (fin) {

		getline(fin, line);								// Read a Line from File
		lineNumber += 1;

		if (lineNumber == 2)
		{
			myCVRP.setNumOfTrucks(NumOfTrucks);
		}

		if (lineNumber == 4)
		{
			dimension = FindDimension(line);
			myCVRP.setDimension(dimension);
		}

		if (lineNumber == 6)
		{
			int capacity = FindCapacity(line);
			myCVRP.setCapacity(capacity);
		}

		if (lineNumber >= 8 && lineNumber < 8 + dimension)
		{
			vec2 myCoordinate = FindCoordinates(line);
			myCVRP.getCoordinates().push_back(myCoordinate);
		}

		if (lineNumber > 8 + dimension && lineNumber <= 8 + dimension + dimension)
		{
			int myDemand = FindDemands(line);
			myCVRP.getCoordinates()[index].setDemand(myDemand);
			index += 1;
		}
	}

	GenerateEdges(myCVRP);
	fin.close();
}

void Init::LoadMDKP(MDKP & myMDKP, std::string fileName)
{
	int lineNumber = 0, numOfKnapsacks = 0 , numOfObjects = 0;
	int flag = 1;
	std::vector<knapsack> knapsacks;
	std::vector<int> values;
	std::vector<int> tempWeights;
	std::vector<std::vector<int>> weights;
	std::vector<int> capacities;
	std::string line;
	std::ifstream fin;
	std::string filePath("C:\\ArtificialIntelligenceLab\\ArtificialIntelligenceLab\\LinearProgrammingRelaxation\\problems\\");
	fin.open(filePath + fileName, std::ios::in);

	while (fin) {

		getline(fin, line);								// Read a Line from File
		lineNumber += 1;

		if (lineNumber == 1)
		{
			numOfKnapsacks = FindNumOfKnapsacks(line);
			myMDKP.setNumOfKnapsacks(numOfKnapsacks);
			numOfObjects = FindNumOfObjects(line);
			myMDKP.setNumOfObjects(numOfObjects);
		}

		if (lineNumber > 1 && flag == 1)
		{
			std::vector<int> returnedValues = FindValuesOfKnapsacks(line,numOfObjects);
			values.insert(values.end(), returnedValues.begin(), returnedValues.end());
		
			if (values.size() == numOfObjects) {
				flag = 2;
				continue;
			}
		}

		if (flag == 2)
		{
			std::vector<int> returnedCapacity = FindCapacityOfKnapsacks(line,numOfKnapsacks);
			capacities.insert(capacities.end(), returnedCapacity.begin(), returnedCapacity.end());

			if (capacities.size() == numOfKnapsacks) {
				flag = 3;
				continue;
			}		
		}

		if (flag == 3)
		{
			std::vector<int> returnedWeights = FindWeightsOfKnapsacks(line,numOfObjects);

			tempWeights.insert(tempWeights.end(), returnedWeights.begin(), returnedWeights.end());


			if (tempWeights.size() == numOfObjects) {
				weights.push_back(tempWeights);
				tempWeights.clear();
			}

		
			if (weights.size() == numOfKnapsacks)
			{
				break;
			}
		}

	}

	for (int i = 0; i < numOfKnapsacks; i++)
	{
		knapsack sack;
		sack.values = values;
		sack.weights = weights[i];
		sack.capacity = capacities[i];
		knapsacks.push_back(sack);
	}


	myMDKP.setKnapsacks(knapsacks);

	fin.close();
}

int Init::FindDimension(std::string line)
{

	std::size_t pos = line.find(":");				// position of ":" in line
	std::string dim = line.substr(pos + 2);			// get from ":" to the end (':' not included)

	return stoi(dim);

}

int Init::FindCapacity(std::string line)
{
	return FindDimension(line);
}


vec2 Init::FindCoordinates(std::string line)
{

	std::size_t pos = line.find(" ");					// position of "space" in line
	std::string coordinate = line.substr(pos + 1);		// get the coordinates
	pos = coordinate.find(" ");							// position of "space" in coordinate
	int x = stoi(coordinate.substr(0, pos));
	int y = stoi(coordinate.substr(pos + 1));
	vec2 myCoordinate(x, y);
	myCoordinate.setIndex(coordinateIndex);
	coordinateIndex++;
	return myCoordinate;

}

int Init::FindDemands(std::string line)
{
	std::size_t pos = line.find(" ");					// position of "space" in line
	std::string demand = line.substr(pos + 1);			// get the demand

	return stoi(demand);

}

int Init::FindNumOfKnapsacks(std::string line)
{
	return stoi(line);
}

int Init::FindNumOfObjects(std::string line)
{
	std::size_t pos1 = line.find(std::to_string(stoi(line)));						// position of ":" in line
	char arr[] = " ";
	std::size_t pos2 = line.find(arr, pos1 + 1);
	std::string numOfObjects = line.substr(pos2);			// get from ":" to the end (':' not included)

	return stoi(numOfObjects);
}

std::vector<int> Init::FindValuesOfKnapsacks(std::string line, int numOfObjects)
{
	std::replace(line.begin(), line.end(), '\t', ' ');
	std::vector<int> values;
	std::size_t pos1 = 0;								// position of ":" in line

	while (1) {

		if (pos1>= line.length())
		{
			break;
		}

		if (line[pos1] == ' ')
		{
			pos1++;
		}

		else
		{
			char arr[] = " ";
			std::size_t pos2 = line.find(arr, pos1 + 1);
			std::string value = line.substr(pos1, pos2 - pos1);			// get from ":" to the end (':' not included)
			pos1 = pos2;
			values.push_back(stoi(value));

		}
	}

	return values;
}

std::vector<int> Init::FindCapacityOfKnapsacks(std::string line, int numOfKnapsacks)
{
	std::replace(line.begin(), line.end(), '\t', ' ');
	std::vector<int> capacities;
	std::size_t pos1 = 0;								// position of ":" in line
	while (1) {

		if (pos1 >= line.length())
		{
			break;
		}

		if (line[pos1] == ' ')
		{
			pos1++;
		}

		else
		{
			char arr[] = " ";
			std::size_t pos2 = line.find(arr, pos1 + 1);
			std::string capacity = line.substr(pos1, pos2 - pos1);			// get from ":" to the end (':' not included)

			pos1 = pos2;
			capacities.push_back(stoi(capacity));
		}
	}
	return capacities;
}

std::vector<int> Init::FindWeightsOfKnapsacks(std::string line, int numOfObjects)
{
	std::replace(line.begin(), line.end(), '\t', ' ');
	std::vector<int> weights;
	std::size_t pos1 = 0;								// position of ":" in line
	while (1) {

		if (pos1 >= line.length())
		{
			break;
		}

		if (line[pos1] == ' ')
		{
			pos1++;
		}

		else
		{
			char arr[] = " ";
			std::size_t pos2 = line.find(arr, pos1 + 1);
			std::string weight = line.substr(pos1, pos2 - pos1);			// get from ":" to the end (':' not included)
			pos1 = pos2;
			weights.push_back(stoi(weight));
		}
	}
	return weights;
}

void Init::GenerateEdges(CVRP & myCVRP)
{
	std::vector<vec2> myCoordinates = myCVRP.getCoordinates();

	for (size_t i = 0; i < myCoordinates.size(); i++)
	{
		for (size_t j = i + 1; j < myCoordinates.size(); j++)
		{
			myCVRP.getEdges().push_back(Edge(myCoordinates[i], myCoordinates[j]));
		}
	}
}
