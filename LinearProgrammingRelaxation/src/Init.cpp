#include "Init.h"
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

void Init::GenerateEdges(CVRP & myCVRP)
{
	std::vector<vec2> myCoordinates = myCVRP.getCoordinates();

	for (int i = 0; i < myCoordinates.size(); i++)
	{
		for (int j = i + 1; j < myCoordinates.size(); j++)
		{
			myCVRP.getEdges().push_back(Edge(myCoordinates[i], myCoordinates[j]));
		}
	}
}
