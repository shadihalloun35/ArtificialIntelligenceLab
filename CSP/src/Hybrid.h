#pragma once
#include "Base.h"
#include "Node.h"
#include <vector> 
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
using namespace std;

struct ga_struct
{
	int* colors;
	int fitness;					// its fitness
	int age = 0;					// age
};

typedef vector<ga_struct> ga_vector;				// for brevity

class Hybrid : public Base
{

public:

	static void orderMutate(ga_struct &member, int tsize);
	static void crossoverNmutate(ga_vector &population, ga_vector &buffer, int* pointers, int tsize);
	static void mate(ga_vector &population, ga_vector &buffer, int tsize, int maxColor);
	static int calcBj(ga_struct &member, Matrix* mtx, int color);
	static int calcCj(ga_struct &member, Matrix* mtx, int color);
	static int calc_fitness(ga_vector &population, Matrix* mtx, int maxColors);
	static void init_population(ga_vector &population, ga_vector &buffer, int tsize, int maxColor);
	static void GeneticAlgorithm(Matrix* mtx, std::chrono::duration <double> timeAllowed);
	static void GreedyAlgorithm(Matrix* mtx, Node* nodes);
	static void PrintHybridNodes(Matrix & mtx, Node* nodes);
	static void CreateHybridNodes(Node* nodes, int size);
	static void ActivateHybrid();
};
