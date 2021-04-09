// GeneticAlgorithm.cpp : Defines the entry point for the console application.
//


#pragma warning(disable:4786)		// disable debug warning
#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <math.h>					// for abs()
#include "Soulution.h"
#include "Utillis.h"
#include "GeneticAlgorithm.h"

#define GA_POPSIZE		2048		// ga population size
#define GA_MAXITER		100			// maximum iterations
#define GA_ELITRATE		0.1f		// elitism rate
#define GA_MUTATIONRATE	0.7f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define SELECTION	1				// for selecting parents method
#define	K	5						// Tournament size 
#define MAX_AGE	10					// Maximum age of a citizen
#define CROSSOVER	1				// for cross over method ( Partially Matched crossover or Ordered crossover )
#define MUTATION	1				// for mutation method (exchange mutation or insertion mutation)

using namespace std;						  // polluting global namespace, but hey...
typedef vector<ga_struct> ga_vector;		  // for brevity
vector<float> averages;					      // for averages
vector<float> deviations;					  // for standard deviations
int N;
Problem myProblem1;
std::vector<vec2> initialCoordinates;

void init_population(ga_vector &population,
	ga_vector &buffer)
{
	initialCoordinates = myProblem1.getCoordinates();
	N = myProblem1.getDimension();
	for (int i = 0; i < GA_POPSIZE; i++) {
		ga_struct citizen;
		Soulution soulution;
		std::vector<std::vector<vec2>> bestSolution = Utillis::GenerateInitialSolution(myProblem1);	// generating initial soulution
		soulution.setNumOfCarsAllowed(myProblem1.getNumOfTrucks());
		soulution.setPermutation(myProblem1.getCoordinates());
		float solutionCost = Utillis::CalcTourDistance(bestSolution);
		Utillis::UpdateSolution(soulution, bestSolution, solutionCost);
		citizen.fitness = 0;
		citizen.age = 1;
		citizen.permutation = myProblem1.getCoordinates();										    
		citizen.soulution = soulution;
		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}


void printSoulution(Soulution soulution) {
		soulution.setNumOfCarsAllowed(myProblem1.getNumOfTrucks());
		cout << soulution;
}

void calc_fitness(ga_vector &population)
{
	float fitness;
	float average = 0;
	float deviation = 0;


	for (int i = 0; i < GA_POPSIZE; i++) {
		fitness = Utillis::CalcTourDistance(population[i].soulution.getTrucksTour());
		population[i].fitness = fitness;
		average += fitness;
	}

	average = average / GA_POPSIZE;			//calculating the average
	averages.push_back(average);

	for (int i = 0; i < GA_POPSIZE; i++) {

		deviation += pow(population[i].fitness - average, 2);
	}

	deviation = sqrt(deviation / GA_POPSIZE);		   //calculating the std deviation
	deviations.push_back(deviation);
}


bool fitness_sort(ga_struct x, ga_struct y)
{

	return (x.fitness < y.fitness);
}

inline void sort_by_fitness(ga_vector &population)
{
	sort(population.begin(), population.end(), fitness_sort);
}

void elitism(ga_vector &population,
	ga_vector &buffer, int esize)
{
	for (int i = 0; i < esize; i++) {
		buffer[i].permutation = population[i].permutation;
		buffer[i].soulution = population[i].soulution;
		buffer[i].fitness = population[i].fitness;
		buffer[i].age += 1;
	}
}

std::vector<vec2> swapIndecies(std::vector<vec2> arr, int ipos1, int ipos2)
{
	vec2 temp = arr[ipos1];
	arr[ipos1] = arr[ipos2];
	arr[ipos2] = temp;

	return arr;
}

void exchangeMutation(ga_struct &member)
{
	int ipos1 = 0, ipos2 = 0;

	do {
		ipos1 = rand() % N;					
		ipos2 = rand() % N;
	} while (ipos1 == 0 || ipos2 == 0);

	member.permutation = swapIndecies(member.permutation, ipos1, ipos2);

	myProblem1.setCoordinates(member.permutation);
	std::vector<std::vector<vec2>> bestSolution = Utillis::TrucksClassification(myProblem1);
	member.soulution.setPermutation(member.permutation);
	float solutionCost = Utillis::CalcTourDistance(bestSolution);
	Utillis::UpdateSolution(member.soulution, bestSolution, solutionCost);
}

void insertionMutation(ga_struct &member)
{
	int ipos1 = 0, ipos2 = 0;

	do {
		ipos1 = rand() % N;
		ipos2 = rand() % N;
	} while (ipos1 == 0 || ipos2 == 0);

	int temp = ipos1;
	vec2 myVec;
	ipos1 = min(ipos1, ipos2), ipos2 = max(temp, ipos2);

	std::vector<vec2> myArr;

	for (int i = 0; i < ipos1; i++)
	{
		myArr.push_back(member.permutation[i]);
	}

	myVec = member.permutation[ipos1];

	for (int i = ipos1; i < ipos2; i++)
	{
		myArr.push_back(member.permutation[i + 1]);
	}

	myArr.push_back(myVec);

	for (int i = ipos2 + 1; i < N; i++)
	{
		myArr.push_back(member.permutation[i]);
	}

	member.permutation = myArr;
	myProblem1.setCoordinates(member.permutation);
	std::vector<std::vector<vec2>> bestSolution = Utillis::TrucksClassification(myProblem1);
	member.soulution.setPermutation(member.permutation);
	float solutionCost = Utillis::CalcTourDistance(bestSolution);
	Utillis::UpdateSolution(member.soulution, bestSolution, solutionCost);
}

void mutate(ga_struct &member)
{
	switch (MUTATION)
	{
	case 1:
		exchangeMutation(member);			// exchange (swap) mutation
		break;

	case 2:
		insertionMutation(member);		   // insertion mutation
		break;

	default:
		break;
	}

}

int* Naïve()
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);

	if (CROSSOVER == 1)										// this method produces 2 children instead of one child
		numOfParents = (GA_POPSIZE - esize);

	int *parents = new int[numOfParents];

	for (int i = 0; i < numOfParents; i++) {
		*(parents + i) = rand() % (GA_POPSIZE / 2);
	}

	return parents;
}

int* RWS(ga_vector &population, int *points, int* newFitness)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);
	if (CROSSOVER == 1)										// this method produces 2 children instead of one child
		numOfParents = (GA_POPSIZE - esize);

	int *parents = new int[numOfParents];									// the selected parents
	int* sumFitness = new int[GA_POPSIZE];								    // sum of the fitness till index i
	sumFitness[0] = newFitness[0];
	for (int i = 1; i < GA_POPSIZE; i++) {
		sumFitness[i] = sumFitness[i - 1] + newFitness[i];

	}
	for (int i = 0; i < numOfParents; i++) {		// Roulette Wheel Selection 
		int j = 0;
		while (1)
		{
			if (sumFitness[j] >= *(points + i))
				break;
			j++;

			if (j >= GA_POPSIZE)

			{
				j = GA_POPSIZE - 1;
				break;
			}
		}
		if (j >= GA_POPSIZE)
			j = GA_POPSIZE - 1;
		*(parents + i) = j;
	}
	return parents;
}

int* SUS(ga_vector &population, long totalFitness, int* newFitness)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);
	if (CROSSOVER == 1)										// this method produces 2 children instead of one child
		numOfParents = (GA_POPSIZE - esize);

	int *parents = new int[numOfParents];			    // the selected parents
	int distance = totalFitness / numOfParents;	        // distance between the pointers
	int start = rand() % (distance + 1);			    // random number between 0 and distance
	int *points = new int[numOfParents];		    // list of (sorted)random numbers from 0 to the total fitness

	for (int i = 0; i < numOfParents; i++) {				// points is a (sorted) list of	random numbers														   
		*(points + i) = start + (i * distance);		        // from 0 to total fitness with constant steps. 
	}

	return RWS(population, points, newFitness);

}


void Scaling(ga_vector &population)
{
	float a = population[0].fitness, b = population[0].fitness;						  // our constants

	for (int i = 0; i < GA_POPSIZE; i++) {

		a = min(population[i].fitness, a);
		b = max(population[i].fitness, b);
	}

	for (int i = 0; i < GA_POPSIZE; i++) {

		population[i].fitness = static_cast<float>(0.2 * population[i].fitness + 10); // linear transformation
	}

}


int PlayTournament(ga_vector &population, int* players) {

	int winner = players[0];

	for (int i = 0; i < K; i++) {
		if (population[players[i]].fitness < population[winner].fitness)
			winner = players[i];
	}

	return winner;
}

int* Tournament(ga_vector &population)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);

	if (CROSSOVER == 1)							    // this method produces 2 children instead of one child
		numOfParents = (GA_POPSIZE - esize);

	int *parents = new int[numOfParents];			// the selected parents
	int* players = new int[K];						// K players in the tournament

	for (int i = 0; i < numOfParents; i++) {
		for (int j = 0; j < K; j++) {
			players[j] = rand() % GA_POPSIZE;
		}

		parents[i] = PlayTournament(population, players);
	}

	return parents;
}


int* Aging(ga_vector &population)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int tsize = N;
	int numOfParents = 2 * (GA_POPSIZE - esize);

	if (CROSSOVER == 1)								// this method produces 2 children instead of one child
		numOfParents = (GA_POPSIZE - esize);

	int *parents = new int[numOfParents];			// the selected parents


	for (int i = esize; i < GA_POPSIZE; i++) {
		if (population[i].age > MAX_AGE) {
			ga_struct citizen;
			citizen.fitness = 0;
			citizen.age = 0;										//reset age
			citizen.permutation = initialCoordinates;
			citizen.soulution = Utillis::GenerateInitialSolution(myProblem1);
			population[i] = citizen;
		}
	}

	for (int i = 0; i < numOfParents; i++) {

		while (1)
		{
			parents[i] = rand() % GA_POPSIZE;

			if (population[parents[i]].age > 0)
				break;
		}
	}

	return parents;

}



int* selectParents(ga_vector &population)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);

	if (CROSSOVER == 1)										// this method produces 2 children instead of one child
		numOfParents = (GA_POPSIZE - esize);

	int *parents = new int[numOfParents];			// the selected parents
	int *points = new int[numOfParents];			// list of (sorted)random numbers from 0 to the total fitness

	int* newFitness = new int[GA_POPSIZE];									// the original fitness doesn't work good
	for (int i = 0; i < GA_POPSIZE; i++) {
		newFitness[i] = ((-1)*(population[i].fitness) + population[GA_POPSIZE - 1].fitness);
	}

	long totalFitness = 0;
	for (int i = 0; i < GA_POPSIZE; i++) {
		totalFitness += newFitness[i];
	}

	switch (SELECTION)
	{
	case 1:
		parents = Naïve();					 // naive method for selecting parents
		break;

	case 2:									//RWS + Scaling

		Scaling(population);			   // Linear scaling

		for (int i = 0; i < numOfParents; i++) {				// points is a (sorted) list of																   
			*(points + i) = rand() % (totalFitness + 1);		   // random numbers from 0 to total fitness. 
		}
		sort(points, points + numOfParents);						// sorting the array
		parents = RWS(population, points, newFitness);						  // RWS method for selecting parents
		break;

	case 3:								// SUS method for selecting parents
		parents = SUS(population, totalFitness, newFitness);
		break;

	case 4:								// Tournament selection
		parents = Tournament(population);
		break;

	case 5:
		parents = Aging(population);
		break;

	}

	return parents;
}

void PMX(ga_vector &population, ga_struct &member1, ga_struct &member2, int spos, int i1, int i2)
{
	std::vector<vec2> permutation1;
	std::vector<vec2> permutation2;

	for (int i = 0; i < N; i++) {											// producing the children as is their parents
		permutation1.push_back(population[i1].permutation[i]);
		permutation2.push_back(population[i2].permutation[i]);
	}
	member1.permutation = permutation1;
	member2.permutation = permutation2;
	member1.soulution = population[i1].soulution;
	member2.soulution = population[i2].soulution;

	vec2 temp1 = member1.permutation[spos], temp2 = member2.permutation[spos];

	for (int i = 0; i < N; i++)											   // crossover
	{
		if (member1.permutation[i] == temp2)
			member1.permutation[i] = temp1;

		if (member2.permutation[i] == temp1)
			member2.permutation[i] = temp2;
	}

	member1.permutation[spos] = temp2;
	member2.permutation[spos] = temp1;

	myProblem1.setCoordinates(member1.permutation);
	std::vector<std::vector<vec2>> bestSolution = Utillis::TrucksClassification(myProblem1);
	member1.soulution.setPermutation(member1.permutation);
	float solutionCost = Utillis::CalcTourDistance(bestSolution);
	Utillis::UpdateSolution(member1.soulution, bestSolution, solutionCost);

	myProblem1.setCoordinates(member2.permutation);
	bestSolution = Utillis::TrucksClassification(myProblem1);
	member2.soulution.setPermutation(member2.permutation);
	solutionCost = Utillis::CalcTourDistance(bestSolution);
	Utillis::UpdateSolution(member2.soulution, bestSolution, solutionCost);
}

void OX(ga_vector &population, ga_struct &member1, int i1, int i2)
{
	std::vector<vec2> permutation1;
	int *tempArray = new int[N];								// the first 4 elements of the array
	for (int i = 0; i < N; i++)									// will be our random indices to pick
	{
		tempArray[i] = i;

	}

	random_shuffle(tempArray, tempArray + N);
	sort(tempArray + N / 2, tempArray + N);

	int pointer = N / 2;												// the index to fill
	bool flag;															// to break loop

	for (int i = 0; i < N; i++)									        // producing the child as is the first parents
	{
		permutation1.push_back(population[i1].permutation[i]);
	}
	member1.permutation = permutation1;
	for (int i = 0; i < N; i++)									        // crossover
	{
		flag = false;

		for (int j = 0; j < N / 2; j++)
		{
			if (population[i2].permutation[i] == population[i1].permutation[tempArray[j]])
			{
				flag = true;
				break;
			}
		}

		if (flag == true) continue;

		member1.permutation[tempArray[pointer]] = population[i2].permutation[i];
		pointer++;
	}

	myProblem1.setCoordinates(member1.permutation);
	std::vector<std::vector<vec2>> bestSolution = Utillis::TrucksClassification(myProblem1);
	member1.soulution.setPermutation(member1.permutation);
	float solutionCost = Utillis::CalcTourDistance(bestSolution);
	Utillis::UpdateSolution(member1.soulution, bestSolution, solutionCost);
}

void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int tsize = N, spos, i1, i2;
	int *parents = selectParents(population);
	elitism(population, buffer, esize);


	switch (CROSSOVER) {

	case 1:															// PMX crossover

		for (int i = esize; i < GA_POPSIZE - 1; i = i + 2) {
			i1 = parents[i - esize];
			i2 = parents[i - esize + 1];
			spos = rand() % tsize;
			PMX(population, buffer[i], buffer[i + 1], spos, i1, i2);
			if (rand() < GA_MUTATION) mutate(buffer[i]);
			if (rand() < GA_MUTATION) mutate(buffer[i + 1]);
			buffer[i].age = 0;
			buffer[i + 1].age = 0;
		}

		break;

	case 2:															// OX crossover

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = parents[2 * (i - esize)];
			i2 = parents[2 * (i - esize) + 1];
			OX(population, buffer[i], i1, i2);
			if (rand() < GA_MUTATION) mutate(buffer[i]);
			buffer[i].age = 0;
		}
		break;

	}

}

inline void print_best(ga_vector &gav)
{
	//cout << "Best: ";
	printSoulution(gav[0].soulution);
	//cout << " (" << gav[0].fitness << ")" << endl;
}

inline void swap(ga_vector *&population,
	ga_vector *&buffer)
{
	ga_vector *temp = population; population = buffer; buffer = temp;
}

void GeneticAlgorithm::ActivateGeneticAlgorithm(Problem & _myProblem1)
{
	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;
	myProblem1 = _myProblem1;
	int numOfGenerations = 0;
	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;

	for (int i = 0; i < GA_MAXITER; i++) {

		calc_fitness(*population);		// calculate fitness using the given heuristic
		sort_by_fitness(*population);	// sort them
		//print_best(*population);		// print the best one
		mate(*population, *buffer);		// mate the population together		
		swap(population, buffer);		// swap buffers
		numOfGenerations += 1;
		//std::cout << "Average: " << averages[i] << std::endl;
		//std::cout << "Standard Deviation: " << deviations[i] << std::endl;
	}
	//std::cout << "Number of Generations: " << numOfGenerations << std::endl;
	print_best(*population);
}
