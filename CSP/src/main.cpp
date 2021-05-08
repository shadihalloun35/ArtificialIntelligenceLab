#include "BackJumping.h"
#include "ForwardChecking.h"
#define Backward_Forward	1

int main() 
{

	switch (Backward_Forward)
	{
		case 1:
			BackJumping::ActivateBackJumping();
			break;

		case 2:
			ForwardChecking::ActivateForwardChecking();
			break;

		default:
			break;
	}



	return 0;
}