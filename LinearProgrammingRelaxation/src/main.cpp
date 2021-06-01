#include <iostream>					// for cout etc.
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
#include <ctime>					// for clock ticks
#include <stdlib.h>
#include "CVRP.h"
#include "MDKP.h"


using namespace std;	// polluting global namespace, but hey...
int mdkp = 1;	// choose one of the six problems
int cvrp = 1;	// choose one of the seven problems
int type = 1;
float timeAllowed;
CVRP cvrpProblem;
MDKP mdkpProblem;


void InitProblemMDKP()
{
	switch (mdkp)

	{
	case 1:
		mdkpProblem.Initialize("problem1.txt");
		timeAllowed = 40.0f;
		break;

	default:
		break;
	}
}

void InitProblemCVRP()
{
	switch (cvrp)
	{

	case 0:
		cvrpProblem.Initialize("problem0.txt");
		timeAllowed = 20.0f;
		break;

	case 1:
		cvrpProblem.Initialize("problem1.txt");
		timeAllowed = 40.0f;
		break;
	case 2:
		cvrpProblem.Initialize("problem2.txt");
		timeAllowed = 50.0f;
		break;
	case 3:
		cvrpProblem.Initialize("problem3.txt");
		timeAllowed = 100.0f;
		break;
	case 4:
		cvrpProblem.Initialize("problem4.txt");
		timeAllowed = 150.0f;
		break;
	case 5:
		cvrpProblem.Initialize("problem5.txt");
		timeAllowed = 200.0f;
		break;
	case 6:
		cvrpProblem.Initialize("problem6.txt");
		timeAllowed = 250.0f;
		break;
	case 7:
		cvrpProblem.Initialize("problem7.txt");
		timeAllowed = 350.0f;
		break;

	default:
		break;
	}
}

void InputMaintaing()
{
	std::cout << "Press 1 for the MDKP Problem or Press 2 for the CVRP Problem" << std::endl;
	cin >> type;

	if (type == 1)
	{
		std::cout << " Please enter the number of the Problem ( 1 )" << std::endl;
		cin >> mdkp;
		InitProblemMDKP();
	}

	else if (type == 2)
	{
		std::cout << " Please enter the number of the Problem ( 0 - 7 )" << std::endl;
		cin >> cvrp;
		InitProblemCVRP();
	}

}

int main()
{

	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	srand(unsigned(time(NULL)));
	clock_t begin = std::clock();					// for clock ticks
	InputMaintaing();								// choosing the problem and the algorithm
	clock_t end = std::clock();
	float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
	if (timeAllowed < time_spent)
	{
		system("CLS");
		std::cout << "Could not find a soloution" << std::endl;
	}
	std::cout << "Clock Ticks: " << time_spent << "s" << std::endl;
	const sec duration = clock::now() - before;
	std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
	getchar();
	getchar();
	return 0;
}