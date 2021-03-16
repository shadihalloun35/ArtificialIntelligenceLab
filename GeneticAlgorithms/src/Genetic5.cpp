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
#include "Particle.h"

#define GA_POPSIZE		1000		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")
#define C1	2						// for exploration
#define C2	2						// for exploitation
#define W	0.5						// for Inertia



using namespace std;				// polluting global namespace, but hey...
int operatorPoint = 1;				// for Reproduction Operators
int hueristic = 2;					// The Givin Hueristic or Bull's Eye Hueristic



struct ga_struct
{
	string str;						// the string
	unsigned int fitness;		   // its fitness;
};

float average;					// its average
float deviation;			   // its deviation


typedef vector<ga_struct> ga_vector;		 // for brevity
vector<Particle> particle_vector;	        // for particles
string globalBest;						   // for global best

void init_population(ga_vector &population,
	ga_vector &buffer)
{
	int tsize = GA_TARGET.size();

	for (int i = 0; i < GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		citizen.str.erase();
//		citizen.average = 0;
	//	citizen.deviation = 0;

		for (int j = 0; j < tsize; j++)
			citizen.str += (rand() % 90) + 32;

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}

void init_global_best()
{
	int tsize = GA_TARGET.size();
	globalBest.erase();

	for (int j = 0; j < tsize; j++)
		globalBest += (rand() % 90) + 32;
}

void calc_fitness(ga_vector &population)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;
	average = 0;
	deviation = 0;

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

	/**
	for (int i = 0; i < GA_POPSIZE; i++) {		//updating the average and the deviation for each citizen for the current generation

		population[i].average = average;
		population[i].deviation = deviation;
	}
	*/

}

void BullsEye_calc_fitness(ga_vector &population)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;
	average = 0;
	deviation = 0;

	for (int i = 0; i < GA_POPSIZE; i++) {
		fitness = tsize * 10;

		for (int j = 0; j < tsize; j++) {
			if (population[i].str[j] == target[j]) {
				fitness -= 10;
			}

			else {
				for (int k = 0; k < tsize; k++) {
					if (population[i].str[j] == target[k]) {
						fitness -= 1;
						break;
					}
				}
			}
		}
		population[i].fitness = fitness;
		average += fitness;
	}

	average = average / GA_POPSIZE;			//calculating the average

	for (int i = 0; i < GA_POPSIZE; i++) {

		deviation += pow(population[i].fitness - average, 2);
	}

	deviation = sqrt(deviation / GA_POPSIZE);		   //calculating the std deviation

	/**
	for (int i = 0; i < GA_POPSIZE; i++) {		//updating the average and the deviation for each citizen for the current generation

		population[i].average = average;
		population[i].deviation = deviation;
	}
	*/

}

template <class  T>
bool fitness_sort(T x, T y)
{
	/**
	if (std::is_same<T, Particle>::value)
	{
		return (x.get_fitness() < y.get_fitness());

	}

	return (x.fitness < y.fitness);
	*/

	return (x.get_fitness() < y.get_fitness());

}

template <class  T>
inline void sort_by_fitness(T &population)
{
	sort(population.begin(), population.end(), fitness_sort<Particle>);
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

void init_pso()
{

	int tsize = GA_TARGET.size();

	for (int i = 0; i < GA_POPSIZE; i++) {

		Particle particle;
		particle_vector.push_back(particle);

		if (particle.get_fitness() < particle.calc_fitness_particle(globalBest))
		{
			globalBest = particle.get_str();
		}
	}
}

void PSO()
{
	init_pso();
	int tsize = GA_TARGET.size();
	Particle global_particle;

	for (int i = 0; i < GA_MAXITER; i++)
	{
		if (global_particle.calc_fitness_particle(globalBest) == 0) break;

		for (int i = 0; i < GA_POPSIZE; i++) {

			string myVelocity;
			string myStr;

			myVelocity.erase();
			myStr.erase();

			for (int j = 0; j < tsize; j++) {

				double r1 = (double)rand() / (RAND_MAX);
				double r2 = (double)rand() / (RAND_MAX);
				//cout << particle_vector[i].get_velocity() << endl;
				//cout << particle_vector[i].get_str() << endl;
				//cout << particle_vector[i].get_localBest() << endl;
				//cout << globalBest << endl;
				//cout << "--------------\n";

				myVelocity += W * (double)particle_vector[i].get_velocity()[j]
					+ C1 * r1 * (double)(particle_vector[i].get_localBest()[j] - particle_vector[i].get_str()[j])
					+ C2 * r2 * (double)(globalBest[j] - particle_vector[i].get_str()[j]);

				//cout << myVelocity << endl;

				myStr += particle_vector[i].get_str()[j] + particle_vector[i].get_velocity()[j]; //+32

			}

			particle_vector[i].set_velocity(myVelocity);
			particle_vector[i].set_str(myStr);
			particle_vector[i].set_fitness(particle_vector[i].calc_fitness_particle(myStr));


			if (particle_vector[i].get_fitness() 
				< particle_vector[i].calc_fitness_particle(particle_vector[i].get_localBest()))
			{
				particle_vector[i].set_localBest(particle_vector[i].get_str());

				if (particle_vector[i].calc_fitness_particle(particle_vector[i].get_localBest())
					< particle_vector[i].calc_fitness_particle(globalBest))
				{
					globalBest = particle_vector[i].get_localBest();
				}
			}
		}

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

	init_global_best();					//initialize global best
	PSO();

	sort_by_fitness<vector<Particle>>(particle_vector);
	cout << "Best: " << particle_vector[0].get_str() << " (" << particle_vector[0].get_fitness() << ")" << endl;

	
	/**
	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;
	operatorPoint = checkInputOperator();
	for (int i = 0; i < GA_MAXITER; i++) {
		clock_t begin = std::clock();

		switch (hueristic)
		{
		case 1:
			calc_fitness(*population);		// calculate fitness using the given hueristic
			break;

		case 2:
			BullsEye_calc_fitness(*population);		// calculate fitness using Bull's eye hueristic
			break;
		}
		sort_by_fitness(*population);	// sort them
		print_best(*population);		// print the best one

		if ((*population)[0].fitness == 0) break;

		mate(*population, *buffer);		// mate the population together
		swap(population, buffer);		// swap buffers

		clock_t end = std::clock();
		float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
		numOfGenerations += 1;
		std::cout << "Average: " << average << std::endl;
		std::cout << "Standard Deviation: " << deviation << std::endl;

		std::cout << "Clock Ticks: " << time_spent << "s" << std::endl;

	}
	*/

	//std::cout << "Number of Generations: " << numOfGenerations << std::endl;

	const sec duration = clock::now() - before;
	std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;

	getchar();
	getchar();

	return 0;
}