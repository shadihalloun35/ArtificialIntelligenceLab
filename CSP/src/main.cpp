#include "BackJumping.h"
#include "ForwardChecking.h"
#include "Feasibility.h"
#include "Objective.h"
#include "Hybrid.h"


#define Approach	5


int main() 
{

	switch (Approach)
	{
		case 1:
			BackJumping::ActivateBackJumping();
			break;

		case 2:
			ForwardChecking::ActivateForwardChecking();
			break;

		case 3:
			Feasibility::ActivateFeasibility();
			break;

		case 4:
			Objective::ActivateObjective();
			break;

		case 5:
			Hybrid::ActivateHybrid();
			break;


		default:
			break;
	}



	return 0;
}