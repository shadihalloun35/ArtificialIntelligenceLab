// Genetic5.cpp : Defines the entry point for the console application.
//


#pragma warning(disable:4786)		// disable debug warning

#include <iostream>					// for cout etc.
#include <vector>					// for vector class
#include <string>					// for string class
#include <algorithm>				// for sort algorithm
#include <time.h>					// for random seed
#include <math.h>					// for abs()
#include <chrono>					// for elapsed time
#include <ctime>					// for clock ticks


#define GA_POPSIZE		2048		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")

using namespace std;				// polluting global namespace, but hey...
int operatorPoint = 1;

struct ga_struct
{
	string str;						// the string
	unsigned int fitness;			// its fitness
	float average;					// its average
	float deviation;				// its deviation
};

typedef vector<ga_struct> ga_vector;// for brevity

void init_population(ga_vector &population,
	ga_vector &buffer)
{
	int tsize = GA_TARGET.size();

	for (int i = 0; i < GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		citizen.str.erase();
		citizen.average = 0;
		citizen.deviation = 0;

		for (int j = 0; j < tsize; j++)
			citizen.str += (rand() % 90) + 32;

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}

void calc_fitness(ga_vector &population)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;
	float average = 0;
	float deviation = 0;

	for (int i = 0; i < GA_POPSIZE; i++) {
		fitness = 0;
		for (int j = 0; j < tsize; j++) {
			fitness += abs(int(population[i].str[j] - target[j]));
		}

		population[i].fitness = fitness;
		average += fitness;
	}

	average = average / GA_POPSIZE;			//calculating the average

	for (int i = 0; i < GA_POPSIZE; i++) {

		deviation += pow(population[i].fitness - average, 2);
	}

	deviation = sqrt(deviation / GA_POPSIZE);		   //calculating the std deviation


	for (int i = 0; i < GA_POPSIZE; i++) {		//updating the average and the deviation for each citizen for the current generation

		population[i].average = average;
		population[i].deviation = deviation;
	}

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
		buffer[i].str = population[i].str;
		buffer[i].fitness = population[i].fitness;
	}
}

void mutate(ga_struct &member)
{
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32;

	member.str[ipos] = ((member.str[ipos] + delta) % 122);
}

void mate(ga_vector &population, ga_vector &buffer)
{

	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos,spos2, i1, i2;

	elitism(population, buffer, esize);

	switch (operatorPoint) {

	case 1:						// Single Point Operator

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = rand() % (GA_POPSIZE / 2);
			i2 = rand() % (GA_POPSIZE / 2);
			spos = rand() % tsize;

			buffer[i].str = population[i1].str.substr(0, spos) +
				population[i2].str.substr(spos, tsize - spos);

			if (rand() < GA_MUTATION) mutate(buffer[i]);
		}

		break;

	case 2:				// Two Point Operator

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = rand() % (GA_POPSIZE / 2);
			i2 = rand() % (GA_POPSIZE / 2);
			spos = rand() % tsize;
			spos2 = rand() % tsize;

			buffer[i].str = population[i1].str.substr(0, std::min(spos,spos2)) +
				population[i2].str.substr(std::min(spos, spos2), std::max(spos, spos2) - std::min(spos, spos2)) +
				population[i1].str.substr(std::max(spos, spos2), tsize - std::max(spos, spos2));

			if (rand() < GA_MUTATION) mutate(buffer[i]);
		}
		break;

	case 3:				//// Uniform Point Operator

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = rand() % (GA_POPSIZE / 2);
			i2 = rand() % (GA_POPSIZE / 2);
			string myStr;
			myStr.erase();
			for (int j = 0; j < tsize; j++) {
				spos = rand() % 2;

				if (spos == 0)
				{
					myStr += population[i1].str.substr(j, 1);
				}
				if (spos == 1)
				{
					myStr += population[i2].str.substr(j, 1);
				}

			}
			buffer[i].str = myStr;
			if (rand() < GA_MUTATION) mutate(buffer[i]);
		}
		break;
	}

}

inline void print_best(ga_vector &gav)
{
	cout << "Best: " << gav[0].str << " (" << gav[0].fitness << ")" << endl;
}

inline void swap(ga_vector *&population,
	ga_vector *&buffer)
{
	ga_vector *temp = population; population = buffer; buffer = temp;
}

int checkInputOperator()
{
	double operatorInput;

	std::cout << "For Single Point Operator -  Press 1\n";
	std::cout << "For Two Point Operator -  Press 2\n";
	std::cout << "For Uniform Point Operator -  Press 3\n";

	while (1)
	{
		try {

			cin >> operatorInput;

			if (!cin)
			{
				cin.clear(); // reset failbit
				cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				throw new std::string("Invalid Input !, Please Try Again\n");
			}

			if (operatorInput == 1 || operatorInput == 2 || operatorInput == 3)
				break;

			else {
				throw new std::string("Number Must be from 1 to 3!, Please Try Again\n");
			}


		}
		catch (std::string *caught) {
			cout << *caught << endl;
			continue; // go again
		}

	}

	operatorPoint = int(operatorInput);

	return operatorPoint;
}

int main()
{
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();
	int numOfGenerations = 0;
	int operatorPoint;
	srand(unsigned(time(NULL)));

	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;
	operatorPoint = checkInputOperator();

	
	for (int i = 0; i < GA_MAXITER; i++) {
		clock_t begin = std::clock();

		calc_fitness(*population);		// calculate fitness
		sort_by_fitness(*population);	// sort them
		print_best(*population);		// print the best one

		if ((*population)[0].fitness == 0) break;

		mate(*population, *buffer);		// mate the population together
		swap(population, buffer);		// swap buffers

		clock_t end = std::clock();
		float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
		numOfGenerations += 1;
		std::cout << "Average: " << (*population)[0].average << std::endl;
		std::cout << "Standard Deviation: " << (*population)[0].deviation << std::endl;

		std::cout << "Clock Ticks: " << time_spent << "s" << std::endl;

	}

	std::cout << "Number of Generations: " << numOfGenerations << std::endl;

	const sec duration = clock::now() - before;
	std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;

	getchar();
	getchar();

	return 0;
}