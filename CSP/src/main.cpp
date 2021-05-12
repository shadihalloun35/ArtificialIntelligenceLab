#include <iostream>
#include <fstream>
#include <string>
#include "BackJumping.h"
#include "ForwardChecking.h"
#include "Feasibility.h"
#include "Objective.h"
#include "Hybrid.h"

int Approach;
int timeAllowed = 0;
string filePath;

int main() 
{
	std::cout << " Please enter the path of the Problem" << std::endl;
	std::cin >> filePath;
	std::cout << " Press 1 for BACKTRACKING Approach" << std::endl;
	std::cout << " Press 2 for FORWARD CHECKING Approach" << std::endl;
	std::cout << " Press 3 for Feasibilty Approach" << std::endl;
	std::cout << " Press 4 for Objective Approach" << std::endl;
	std::cout << " Press 5 for Hybrid Approach" << std::endl;
	cin >> Approach;

	if (Approach >= 3)
	{
		std::cout << "Please enter the runtime allowed" << std::endl;
		cin >> timeAllowed;
	}

	
	switch (Approach)
	{
		case 1:
			BackJumping::ActivateBackJumping(filePath);
			break;

		case 2:
			ForwardChecking::ActivateForwardChecking(filePath);
			break;

		case 3:
			Feasibility::ActivateFeasibility(filePath, timeAllowed);
			break;

		case 4:
			Objective::ActivateObjective(filePath, timeAllowed);
			break;

		case 5:
			Hybrid::ActivateHybrid(filePath, timeAllowed);
			break;


		default:
			break;
	}

	return 0;
}