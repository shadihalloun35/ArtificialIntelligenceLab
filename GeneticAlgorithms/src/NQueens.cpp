// NQueens.cpp : Defines the entry point for the console application.
//

#pragma warning(disable:4786)		// disable debug warning
/**
#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>					// for abs()
#include <chrono>					// for elapsed time
#include <ctime>					// for clock ticks
#include "NQueens.h"

#define GA_POPSIZE		1000		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")
#define heuristic	1				// The Givin Hueristic or Bull's Eye Hueristic
#define SELECTION	1				// for selecting parents method
#define	K	5						// Tournament size 
#define MAX_AGE	10					// Maximum age of a citizen
#define N	8						// for the size of the board
#define CROSSOVER	1				// for cross over method ( Partially Matched crossover or Ordered crossover )
#define MUTATION	2				// for mutation method (exchange mutation or insertion mutation)
#define algorithm	1				// The given algorithm or minimal conflict





using namespace std;				// polluting global namespace, but hey...



struct ga_struct
{
	int *board = new int[N];	   // the chess board size N
	unsigned int fitness;		   // its fitness;
	unsigned int age;
};



typedef vector<ga_struct> ga_vector;		   // for brevity
vector<float> averages;					      // for averages
vector<float> deviations;					 // for standard deviations


int NQueens::getHueristic()				// get the current heuristic 
{
	return heuristic;
}

void init_game(ga_struct &game)				//for minConflict algorithm
{
	for (int j = 0; j < N; j++)
		game.board[j] = j;

	random_shuffle(game.board, game.board + N);
	game.age = 0;

}

void init_population(ga_vector &population,
	ga_vector &buffer)
{
	int tsize = N;

	for (int i = 0; i < GA_POPSIZE; i++) {
		ga_struct citizen;
		citizen.fitness = 0;
		citizen.age = 1;

		for (int j = 0; j < N; j++)
			citizen.board[j] = j;

		random_shuffle(citizen.board, citizen.board + N);

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}


void printBoard(int* board) {
	for (int i = 0; i < N; i++) {
		cout << board[i] << " ";
	}
}

void calc_fitness(ga_vector &population)
{
	unsigned int fitness;
	float average = 0;
	float deviation = 0;


	for (int i = 0; i < GA_POPSIZE; i++) {
		fitness = 0;
		for (int j = 0; j < N - 1; j++) {
			for (int k = j + 1; k < N; k++)
			{
				if ((k - j) == abs(population[i].board[j] - population[i].board[k]))
					fitness += 1;
			}
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

void game_calc_fitness(ga_struct &game)
{
	unsigned int fitness;
	fitness = 0;
	for (int j = 0; j < N - 1; j++) {
		for (int k = j + 1; k < N; k++)
		{
			if (game.board[j] == game.board[k])
				fitness++;

			if ((k - j) == abs(game.board[j] - game.board[k]))
				fitness += 1;
		}
	}
	game.fitness = fitness;
}


template <class  S>
bool fitness_sort(S x, S y)
{

	return (x.fitness < y.fitness);
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
		buffer[i].board = population[i].board;
		buffer[i].fitness = population[i].fitness;
		buffer[i].age += 1;
	}
}

int *swapIndecies(int * arr, int ipos1, int ipos2)
{
	int temp = *(arr + ipos1);
	*(arr + ipos1) = *(arr + ipos2);
	*(arr + ipos2) = temp;

	return arr;
}

void exchangeMutation(ga_struct &member)
{
	int ipos1 = rand() % N;
	int ipos2 = rand() % N;

	member.board = swapIndecies(member.board, ipos1, ipos2);
}

void insertionMutation(ga_struct &member)
{
	int ipos1 = rand() % N;
	int temp = ipos1;
	int ipos2 = rand() % N;

	ipos1 = min(ipos1, ipos2), ipos2 = max(temp, ipos2);

	int * myArr = new int[N];

	for (int i = 0; i < ipos1; i++)
	{
		*(myArr + i) = member.board[i];
	}

	temp = *(myArr + ipos1);

	for (int i = ipos1; i < ipos2; i++)
	{
		*(myArr + i) = member.board[i + 1];
	}

	*(myArr + ipos2) = temp;

	for (int i = ipos2 + 1; i < N; i++)
	{
		*(myArr + i) = member.board[i];
	}


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

int* RWS(ga_vector &population, int *points)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);

	if (CROSSOVER == 1)								  // this method produces 2 children instead of one child
		numOfParents = (GA_POPSIZE - esize);

	int *parents = new int[numOfParents];			 // the selected parents


	for (int i = 0; i < numOfParents; i++) {		// Roulette Wheel Selection 
		int sumFitness = 0;
		int j = 0;

		while (1)
		{
			sumFitness += population[j].fitness;
			if (sumFitness >= *(points + i))
				break;
			j++;

			if (j > GA_POPSIZE)

			{
				j = GA_POPSIZE;
				break;
			}
		}

		if (j > GA_POPSIZE)
			j = GA_POPSIZE;

		*(parents + i) = j;
	}

	return parents;

}

int* SUS(ga_vector &population, int totalFitness)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int numOfParents = 2 * (GA_POPSIZE - esize);

	if (CROSSOVER == 1)									// this method produces 2 children instead of one child
		numOfParents = (GA_POPSIZE - esize);

	int *parents = new int[numOfParents];			    // the selected parents
	int distance = totalFitness / numOfParents;	        // distance between the pointers
	int start = rand() % (distance + 1);			    // random number between 0 and distance
	int *points = new int[numOfParents];		        // list of (sorted)random numbers from 0 to the total fitness

	for (int i = 0; i < numOfParents; i++) {				// points is a (sorted) list of	random numbers														   
		*(points + i) = start + (i * distance);		        // from 0 to total fitness with constant steps. 
	}

	return RWS(population, points);

}


void Scaling(ga_vector &population)
{
	unsigned int a = population[0].fitness, b = population[0].fitness;					//our constants

	for (int i = 0; i < GA_POPSIZE; i++) {

		a = min(population[i].fitness, a);
		b = max(population[i].fitness, b);
	}

	for (int i = 0; i < GA_POPSIZE; i++) {

		population[i].fitness = 0.2 * population[i].fitness + 10;				// linear transformation
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

			for (int j = 0; j < tsize; j++) {
				citizen.board[j] = j;
			}

			random_shuffle(citizen.board, citizen.board + N);

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
	int totalFitness = 0;

	for (int i = 0; i < GA_POPSIZE; i++) {
		totalFitness += population[i].fitness;
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
		parents = RWS(population, points);								   // RWS method for selecting parents
		break;

	case 3:								// SUS method for selecting parents
		parents = SUS(population, totalFitness);
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

	for (int i = 0; i < N; i++) {											// producing the children as is their parents
		member1.board[i] = population[i1].board[i];
		member2.board[i] = population[i2].board[i];

	}

	int temp1 = member1.board[spos], temp2 = member2.board[spos];


	for (int i = 0; i < N; i++)											   // crossover
	{
		if (member1.board[i] == temp2)
			member1.board[i] = temp1;

		if (member2.board[i] == temp1)
			member2.board[i] = temp2;
	}
	member1.board[spos] = temp2;
	member2.board[spos] = temp1;

}

void OX(ga_vector &population, ga_struct &member1, int i1, int i2)
{

	int *tempArray = new int[N];									// the first 4 elements of the array
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
		member1.board[i] = population[i1].board[i];
	}

	for (int i = 0; i < N; i++)									        // crossover
	{
		flag = false;

		for (int j = 0; j < N / 2; j++)
		{
			if (population[i2].board[i] == population[i1].board[tempArray[j]])
			{
				flag = true;
				break;
			}

		}

		if (flag == true) continue;

		member1.board[tempArray[pointer]] = population[i2].board[i];
		pointer++;
	}

}

void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int tsize = N, spos, i1, i2;
	int *parents = selectParents(population);
	elitism(population, buffer, esize);


	switch (CROSSOVER) {

	case 1:															// PMX crossover

		for (int i = esize; i < GA_POPSIZE; i = i + 2) {
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
	cout << "Best: ";
	printBoard(gav[0].board);
	cout << " (" << gav[0].fitness << ")" << endl;
}

inline void swap(ga_vector *&population,
	ga_vector *&buffer)
{
	ga_vector *temp = population; population = buffer; buffer = temp;
}

void minimalConflict(ga_struct &game)
{
	int* randQueens = new int[N];
	int queen = (rand() % N);
	int previousFitness = game.fitness;
	int previousQueen = game.board[queen];

	for (int i = 0; i < N; i++) {
		randQueens[i] = i;
	}
	random_shuffle(randQueens, randQueens + N);				//moving the queens

	game.board[queen] = randQueens[0];
	game_calc_fitness(game);

	int currentFitness = game.fitness;
	int chosenQueen = randQueens[0];

	for (int i = 1; i < N; i++)									// making a move
	{
		game.board[queen] = randQueens[i];
		game_calc_fitness(game);

		if (game.fitness < currentFitness)						// good move
		{
			currentFitness = game.fitness;
			chosenQueen = randQueens[i];
		}
	}

	if (currentFitness >= previousFitness)						// bad move
	{
		game.board[queen] = previousQueen;
		game_calc_fitness(game);								
	}

	game.board[queen] = chosenQueen;
	game_calc_fitness(game);									// updating the fitness
}

int main()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	int numOfGenerations = 0;
	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;
	srand(unsigned(time(NULL)));

	switch (algorithm)
	{
	case 1:
		init_population(pop_alpha, pop_beta);
		population = &pop_alpha;
		buffer = &pop_beta;
		for (int i = 0; i < GA_MAXITER; i++) {

			clock_t begin = std::clock();		// for clock ticks
			calc_fitness(*population);		// calculate fitness using the given heuristic
			sort_by_fitness<vector<ga_struct>, ga_struct>(*population);	// sort them
			print_best(*population);		// print the best one
			if ((*population)[0].fitness == 0) break;

			mate(*population, *buffer);		// mate the population together		
			swap(population, buffer);		// swap buffers
			clock_t end = std::clock();
			float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
			numOfGenerations += 1;
			std::cout << "Average: " << averages[i] << std::endl;
			std::cout << "Standard Deviation: " << deviations[i] << std::endl;

			std::cout << "Clock Ticks: " << time_spent << "s" << std::endl;

		}

		std::cout << "Number of Generations: " << numOfGenerations << std::endl;
		break;

	case 2:

		ga_struct game;
		init_game(game);

		for (int i = 0; i < GA_MAXITER; i++) {
			if (game.fitness == 0) {
				break;
			}
			minimalConflict(game);
			numOfGenerations += 1;
		}
		printBoard(game.board);

		std::cout << "Number of Generations: " << numOfGenerations << std::endl;
		break;

	}





	const sec duration = clock::now() - before;
	std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;

	getchar();
	getchar();

	return 0;
}
*/