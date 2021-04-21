#include <iostream>					// for cout etc.
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
#include <ctime>					// for clock ticks
#include "Problem.h"
#include "SimulatedAnnealing.h"
#include "TabuSearch.h"
#include "AntColonyOptimization.h"
#include "GeneticAlgorithm.h"
#include <stdlib.h>


using namespace std;								// polluting global namespace, but hey...

Problem myProblem;
float timeAllowed;

int problem = 1;	// choose one of the seven problems
int Algorithm = 1;  // choose local search algorithm or genetic algorithm
int MetaHeuristicAlgorithm = 3; // choose one of the local search algorithms


void InitProblem()
{
	switch (problem)
	{

	case 0:
		myProblem.Initialize("problem0.txt");
		timeAllowed = 20.0f;
		break;

	case 1:
		myProblem.Initialize("problem1.txt");
		timeAllowed = 40.0f;
		break;
	case 2:
		myProblem.Initialize("problem2.txt");
		timeAllowed = 50.0f;
		break;
	case 3:
		myProblem.Initialize("problem3.txt");
		timeAllowed = 100.0f;
		break;
	case 4:
		myProblem.Initialize("problem4.txt");
		timeAllowed = 150.0f;
		break;
	case 5:
		myProblem.Initialize("problem5.txt");
		timeAllowed = 200.0f;
		break;
	case 6:
		myProblem.Initialize("problem6.txt");
		timeAllowed = 250.0f;
		break;
	case 7:
		myProblem.Initialize("problem7.txt");
		timeAllowed = 350.0f;
		break;
	
	default:
		break;
	}
}

void LocalSearch()
{
	switch (MetaHeuristicAlgorithm)
	{

	case 1:
		SimulatedAnnealing::ActivateSimulatedAnnealing(myProblem);
		break;

	case 2:
		TabuSearch::ActivateTabuSearch(myProblem);
		break;

	case 3:
		AntColonyOptimization::ActivateAntColonyOptimization(myProblem);
		break;
	}
}

void FindBestPath()
{
	switch (Algorithm)
	{
	case 1:
		LocalSearch();
		break;

	case 2:
		GeneticAlgorithm::ActivateGeneticAlgorithm(myProblem);
		break;
	}
}

void InputMaintaing()
{
	std::cout << " Please enter the number of the Problem ( 0 -7 )" << std::endl;
	cin >> problem;

	std::cout << " Press 1 for Local Search or Press 2 for Global Search (Genetic algorithm)" << std::endl;
	cin >> Algorithm;

	if (Algorithm == 1)
	{
		std::cout << " Choose Local search algorithm :" << std::endl;
		std::cout << " Press 1 for Simulated annealing" << std::endl;
		std::cout << " Press 2 for TabuSearch" << std::endl;
		std::cout << " Press 3 for ACO" << std::endl;
		cin >> MetaHeuristicAlgorithm;
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
	InitProblem();
	FindBestPath();
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

