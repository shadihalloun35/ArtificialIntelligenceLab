#include <iostream>					// for cout etc.
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
#include <ctime>					// for clock ticks
#include "Problem.h"

#define problem		1				// choose one of the seven problems


using namespace std;				// polluting global namespace, but hey...

Problem myProblem;

void InitProblem()
{
	switch (problem)
	{

	case 1:
		myProblem.Initialize("problem1.txt");
	case 2:
		myProblem.Initialize("problem2.txt");
	case 3:
		myProblem.Initialize("problem3.txt");
	case 4:
		myProblem.Initialize("problem4.txt");
	case 5:
		myProblem.Initialize("problem5.txt");
	case 6:
		myProblem.Initialize("problem6.txt");
	case 7:
		myProblem.Initialize("problem7.txt");
	
	default:
		break;
	}
}

int main()
{
	
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	srand(unsigned(time(NULL)));
	clock_t begin = std::clock();		// for clock ticks
	InitProblem();
	clock_t end = std::clock();
	float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Clock Ticks: " << time_spent << "s" << std::endl;
	const sec duration = clock::now() - before;
	std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
	getchar();
	return 0;
}

