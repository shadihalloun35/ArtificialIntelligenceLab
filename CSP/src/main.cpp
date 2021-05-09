#include "BackJumping.h"
#include "ForwardChecking.h"
#include "Feasibility.h"

#define Approach	3


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

		default:
			break;
	}



	return 0;
}