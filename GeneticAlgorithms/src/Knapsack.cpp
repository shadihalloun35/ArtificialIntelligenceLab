// Knapsack.cpp : Defines the entry point for the console application.
//
/**/
#pragma warning(disable:4786)		// disable debug warning

#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>					// for abs()
#include <chrono>					// for elapsed time
#include <ctime>					// for clock ticks
#include "Knapsack.h"

#define GA_POPSIZE		1024		// ga population size
#define GA_MAXITER		1024		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")
#define heuristic	1				// The Givin Hueristic or Bull's Eye Hueristic
#define SELECTION	1				// for selecting parents method
#define	K	5						// Tournament size 
#define MAX_AGE	10					// Maximum age of a citizen
#define N	10						// number of items
#define CROSSOVER	1				// cross over method ( one point crossover ,two point crossover or uniform crossover)
#define MAX_WEIGHT	165				// weight


//1-
//Number of Items: 10
//Weight: 165

#define MAX_WEIGHT	165			//weight
#define N	10					//number of items

int weights[] = { 23,31,29,44,53,38,63,85,89,82 };  //array that stores the weight of each item
int profits[] = { 92,57,49,68,60,43,67,84,87,72 };  //array that stores the profit of each item

//Best : {1,1,1,1,0,1,0,0,0,0}

//2-
//Number of Items: 5
//Weight: 26
//#define MAX_WEIGHT	26			//weight
//#define N				5			//number of items
//int weights[] = {12,7,11,8,9};			//array that stores the weight of each item
//int profits[] = {24,13,23,15,16};			//array that stores the profit of each item

//Best :{0,1,1,1,0}


//3-
//Number of Items: 6
//Weight: 190
//#define MAX_WEIGHT	190			//weight
//#define N		6					//number of items
//int weights[] = {56,59,80,64,75,17};			 //array that stores the weight of each item
//int profits[] = {50,50,64,46,50,5};			 //array that stores the profit of each item

//Best :{1,1,0,0,1,0}

//4-
//Number of Items: 7
//Weight: 50
//#define MAX_WEIGHT			50				//weight
//#define N				7						//number of items
//int weights[] = {31,10,20,19,4,3,6};			//array that stores the weight of each item
//int profits[] = {70,20,39,37,7,5,10};			//array that stores the profit of each item

//Best :{1,0,0,1,0,0,0}

//5-
//Number of Items: 8
////Weight: 104
//#define MAX_WEIGHT			104				//weight
//#define N				8						//number of items
//int weights[] = {25,35,45,5,25,3,2,2};		//array that stores the weight of each item
//int profits[] = {350,400,450,20,70,8,5,5};	//array that stores the profit of each item

//Best :{1,0,1,1,1,0,1,1}

//6-
//Number of Items: 7
//Weight: 170//
//#define MAX_WEIGHT			170				//weight
//#define N				7						//number of items
//int weights[] = {41,50,49,59,55,57,60};				//array that stores the weight of each item
//int profits[] = {442,525,511,593,546,564,617};		//array that stores the profit of each item

//Best : {0,1,0,1,0,0,1}

//7-
//Number of Items: 15
//Weight: 750//
//#define MAX_WEIGHT			750			//weight
//#define N				15					//number of items
//int weights[] = {70,73,77,80,82,87,90,94,98,106,110,113,115,118,120};				    //array that stores the weight of each item
//int profits[] = {135,139,149,150,156,163,173,184,192,201,210,214,221,229,240};		//array that stores the profit of each item

//Best :{ 1,0,1,0,1,0,1,1,1,0,0,0,0,1,1}

//8-
//Number of Items: 24
//Weight: 6404180/
//#define MAX_WEIGHT				6404180						//weight
//#define N				24										//number of items
//int weights[] = { 382745,799601,909247,729069,467902,44328,34610,698150,823460,903959,853665,551830,610856,670702,488960,951111,323046,446298,931161,31385,496951,264724,224916,169684 };						//array that stores the weight of each item
//int profits[] = { 825594,1677009,1676628,1523970,943972,97426,69666,1296457,1679693,1902996,1844992,1049289,1252836,1319836,953277,2067538,675367,853655,1826027,65731,901489,577243,466257,369261 };			//array that stores the profit of each item

 //Best :{ 1,1,0,1,1,1,0,0,0,1,1,0,1,0,0,1,0,0,0,0,0,1,1,1 }

using namespace std;				// polluting global namespace, but hey...



struct ga_struct
{
	int *sack = new int[N];		   // the number of items N
	unsigned int fitness;		   // its fitness;
	int weight;					   // the weight of the sack
	unsigned int age;
};



typedef vector<ga_struct> ga_vector;		   // for brevity
vector<float> averages;					      // for averages
vector<float> deviations;					 // for standard deviations


int Knapsack::getHueristic()				// get the current heuristic 
{
	return heuristic;
}

void remove_items(ga_struct &citizen) {
	
	int index = 0;
	int* randomStuff = new int[N];

	for (int i = 0; i < N; i++) {					// to choose a random stuff 
		randomStuff[i] = i;
	}

	random_shuffle(randomStuff, randomStuff + N);
	while (1) {

		if (citizen.weight <= MAX_WEIGHT)
			break;

		while (index < N) {
			if (citizen.sack[randomStuff[index]] == 1)
			{
				citizen.weight = (citizen.weight - weights[randomStuff[index]]);
				citizen.sack[randomStuff[index]] = 0;
				index++;
				break;
			}
			index++;
		}
	}
}

void init_population(ga_vector &population,
	ga_vector &buffer)
{

	for (int i = 0; i < GA_POPSIZE; i++) {
		ga_struct citizen;
		citizen.fitness = 0;
		citizen.weight = 0;
		citizen.age = 1;

		for (int j = 0; j < N; j++)
		{
			citizen.sack[j] = rand() % 2;     //0-1 problem, either it contains the item or not

			if (citizen.sack[j] == 1)
				citizen.weight += weights[j];
		}

		if (citizen.weight > MAX_WEIGHT)
		{
			remove_items(citizen);
		}

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}


void printSack(int* sack) {
	for (int i = 0; i < N; i++) {
		cout << sack[i] << " ";
	}
}

void calc_fitness(ga_vector &population)
{
	unsigned int fitness;
	float average = 0;
	float deviation = 0;


	for (int i = 0; i < GA_POPSIZE; i++) {
		fitness = 0;
		for (int j = 0; j < N ; j++) {
			
			if (population[i].sack[j] == 1)						// higher fitness means better 
				fitness += profits[j];
		}

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

template <class  S>
bool fitness_sort(S x, S y)
{

	return (x.fitness > y.fitness);
}

template <class  T, class S>
inline void sort_by_fitness(T &population)
{
	sort(population.begin(), population.end(), fitness_sort<S>);
}

void elitism(ga_vector &population,
	ga_vector &buffer, int esize)
{
	for (int i = 0; i < esize; i++) {
		buffer[i].sack = population[i].sack;
		buffer[i].fitness = population[i].fitness;
		buffer[i].weight = population[i].weight;
		buffer[i].age += 1;
	}
}

void updateWeight(ga_struct &citizen) {

	int weight = 0;
	for (int i = 0; i < N; i++) {
		if (citizen.sack[i] == 1)
			weight += weights[i];
	}
	citizen.weight = weight;
}

void mutate(ga_struct &member)
{
	int ipos = rand() % N;										// choosing randomly an item to remove or add

	if (member.sack[ipos] == 1)
	{
		member.sack[ipos] = 0;
		member.weight -= weights[ipos];
	}

	else if (member.sack[ipos] == 0)
	{
		member.sack[ipos] = 1;
		member.weight += weights[ipos];

	}

	updateWeight(member);										    // updating the current weight

	if (member.weight > MAX_WEIGHT)
		remove_items(member);
}

int* Naïve()
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);
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
				j = GA_POPSIZE-1;
				break;
			}
		}
		if (j >= GA_POPSIZE)
			j = GA_POPSIZE-1;
		*(parents + i) = j;
	}
	return parents;
}

int* SUS(ga_vector &population, long totalFitness, int* newFitness)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);
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
	unsigned int a = population[0].fitness, b = population[0].fitness;					     // our constants

	for (int i = 0; i < GA_POPSIZE; i++) {

		a = min(population[i].fitness, a);
		b = max(population[i].fitness, b);
	}

	for (int i = 0; i < GA_POPSIZE; i++) {

		population[i].fitness = static_cast<unsigned int>(0.2 * population[i].fitness + 10);// linear transformation
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

	int *parents = new int[numOfParents];			// the selected parents


	for (int i = esize; i < GA_POPSIZE; i++) {
		if (population[i].age > MAX_AGE) {
			ga_struct citizen;
			citizen.fitness = 0;
			citizen.age = 0;										//reset age


			citizen.sack[i] = rand() % 2;    

			if (citizen.sack[i] == 1)
				citizen.weight += weights[i];
		

			if (citizen.weight > MAX_WEIGHT)
			{
				remove_items(citizen);
			}

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
		parents = Naïve();						  // naive method for selecting parents
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


void onePointCrossover(ga_vector &population, ga_struct &member1,int spos,  int i1, int i2)
{
	for (int i = 0; i < spos; i++)
	{
		member1.sack[i] = population[i1].sack[i];
	}
	for (int i = spos; i < N; i++)
	{
		member1.sack[i] = population[i2].sack[i];
	}

	updateWeight(member1);										    // updating the current weight

	if (member1.weight > MAX_WEIGHT)								// make sure the weight is valid
	{
		remove_items(member1);
	}
}

void twoPointCrossover(ga_vector &population, ga_struct &member1, int spos,int spos2, int i1, int i2)
{
	
	for (int i = 0; i < std::min(spos, spos2); i++)
	{
		member1.sack[i] = population[i1].sack[i];
	}
	for (int i = std::min(spos, spos2); i < std::max(spos, spos2); i++)
	{
		member1.sack[i] = population[i2].sack[i];
	}
	for (int i = std::max(spos, spos2); i < N; i++)
	{
		member1.sack[i] = population[i1].sack[i];
	}

	updateWeight(member1);										    // updating the current weight

	if (member1.weight > MAX_WEIGHT)								// make sure the weight is valid
	{
		remove_items(member1);
	}
}

void uniformCrossover(ga_vector &population, ga_struct &member1, int i1, int i2)
{
	for (int i = 0; i < N; i++)
	{
		if (rand() % 2 == 0)
		{
			member1.sack[i] = population[i1].sack[i];
		}

		if (rand() % 2 == 1)
		{
			member1.sack[i] = population[i2].sack[i];
		}
	}

	updateWeight(member1);										    // updating the current weight

	if (member1.weight > MAX_WEIGHT)								// make sure the weight is valid
	{
		remove_items(member1);
	}
}

void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int tsize = N, spos,spos2, i1, i2;
	int *parents = selectParents(population);
	elitism(population, buffer, esize);


	switch (CROSSOVER) {

	case 1:															// one point crossover

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = parents[2 * (i - esize)];
			i2 = parents[2 * (i - esize) + 1];
			spos = rand() % tsize;
			onePointCrossover(population, buffer[i], spos, i1, i2);
			if (rand() < GA_MUTATION) mutate(buffer[i]);
			buffer[i].age = 0;

		}
		break;

	case 2:															// two point crossover

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = parents[2 * (i - esize)];
			i2 = parents[2 * (i - esize) + 1];
			spos = rand() % tsize;
			spos2 = rand() % tsize;
			twoPointCrossover(population, buffer[i], spos, spos2, i1, i2);
			if (rand() < GA_MUTATION) mutate(buffer[i]);
			buffer[i].age = 0;
		}
		break;

	case 3:															// uniform crossover

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = parents[2 * (i - esize)];
			i2 = parents[2 * (i - esize) + 1];
			uniformCrossover(population, buffer[i], i1, i2);
			if (rand() < GA_MUTATION) mutate(buffer[i]);
			buffer[i].age = 0;
		}
		break;

	}

}

inline void print_best(ga_vector &gav)
{
	cout << "Best: ";
	printSack(gav[0].sack);
	cout << "fitness: (" << gav[0].fitness << ")";
	cout << " Weight:  (" << gav[0].weight << ")" << endl;

}

inline void swap(ga_vector *&population,
	ga_vector *&buffer)
{
	ga_vector *temp = population; population = buffer; buffer = temp;
}


int main()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();												// for elapsed time
	int numOfGenerations = 0;
	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;
	srand(unsigned(time(NULL)));

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;
	for (int i = 0; i < GA_MAXITER; i++) {

		clock_t begin = std::clock();												// for clock ticks
		calc_fitness(*population);													// calculate fitness using the given heuristic
		sort_by_fitness<vector<ga_struct>, ga_struct>(*population);					// sort them
		print_best(*population);													// print the best one
		if ((*population)[0].weight == MAX_WEIGHT) break;

		mate(*population, *buffer);													// mate the population together		
		swap(population, buffer);													// swap buffers
		clock_t end = std::clock();
		float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
		numOfGenerations += 1;
		std::cout << "Average: " << averages[i] << std::endl;
		std::cout << "Standard Deviation: " << deviations[i] << std::endl;

		std::cout << "Clock Ticks: " << time_spent << "s" << std::endl;

	}

	std::cout << "Number of Generations: " << numOfGenerations << std::endl;




	const sec duration = clock::now() - before;
	std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;

	getchar();
	getchar();

	return 0;
}
