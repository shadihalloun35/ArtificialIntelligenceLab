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
#include "Genetic5.h"
#include "Particle.h"

#define GA_POPSIZE		2048		// ga population size
#define GA_MAXITER		16384		// maximum iterations
#define GA_ELITRATE		0.10f		// elitism rate
#define GA_MUTATIONRATE	0.25f		// mutation rate
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")
#define C1	1.5						// for exploration
#define C2	1.5						// for exploitation
#define W	0.5						// for Inertia
#define algorithm	1				// The given algorithm or PSO
#define heuristic	1				// The Givin Hueristic or Bull's Eye Hueristic
#define SELECTION	3				// for selecting parents method
#define	K	5						// Tournament size 
#define MAX_AGE	10					// Maximum age of a citizen



using namespace std;				// polluting global namespace, but hey...
int operatorPoint = 1;				// for Reproduction Operators



struct ga_struct
{
	string str;						// the string
	unsigned int fitness;		   // its fitness;
	unsigned int age;
};



typedef vector<ga_struct> ga_vector;		   // for brevity
vector<float> averages;					      // for averages
vector<float> deviations;					 // for standard deviations
vector<Particle> particle_vector;	        // for particles
string globalBest;						   // for global best


int Genetic5::getHueristic()				// get the current heuristic 
{
	return heuristic;
}

void init_population(ga_vector &population,
	ga_vector &buffer)
{
	int tsize = GA_TARGET.size();

	for (int i = 0; i < GA_POPSIZE; i++) {
		ga_struct citizen;

		citizen.fitness = 0;
		citizen.age = 1;
		citizen.str.erase();

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

void Naïve_calc_fitness(ga_vector &population)
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
	averages.push_back(average);

	for (int i = 0; i < GA_POPSIZE; i++) {

		deviation += pow(population[i].fitness - average, 2);
	}

	deviation = sqrt(deviation / GA_POPSIZE);		   //calculating the std deviation
	deviations.push_back(deviation);

}

void BullsEye_calc_fitness(ga_vector &population)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;
	float average = 0;
	float deviation = 0;

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

	average = average / GA_POPSIZE;						//calculating the average
	averages.push_back(average);

	for (int i = 0; i < GA_POPSIZE; i++) {

		deviation += pow(population[i].fitness - average, 2);
	}

	deviation = sqrt(deviation / GA_POPSIZE);		   //calculating the std deviation
	deviations.push_back(deviation);

}

void calc_fitness(ga_vector &population)
{
	switch (heuristic)
	{
	case 1:
		Naïve_calc_fitness(population);		// calculate fitness using the given heuristic
		break;

	case 2:
		BullsEye_calc_fitness(population);		// calculate fitness using Bull's eye heuristic
		break;
	}
}


template <class  S>
bool fitness_sort(S x, S y)
{
	
	if (is_same<S, Particle>::value == true)					// for particles, fitness variable is private
	{
		//cout << "a";
		//return (x.get_fitness() < y.get_fitness());
	}

	return (x.fitness < y.fitness);

}

template <class  T,class S>
inline void sort_by_fitness(T &population)
{
	sort(population.begin(), population.end(), fitness_sort<S>);
}

void elitism(ga_vector &population,
	ga_vector &buffer, int esize)
{
	for (int i = 0; i < esize; i++) {
		buffer[i].str = population[i].str;
		buffer[i].fitness = population[i].fitness;
		buffer[i].age += 1;
	}
}

void mutate(ga_struct &member)
{
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32;

	member.str[ipos] = ((member.str[ipos] + delta) % 122);
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

int* RWS(ga_vector &population,int *points, int* newFitness)
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
			if(sumFitness[j] >= *(points + i))
				break;
			j++;
		}

		*(parents + i) = j;
	}

	return parents;

}


int* SUS(ga_vector &population,long totalFitness,int* newFitness)
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

	return RWS(population, points,newFitness);								   

}


void Scaling(ga_vector &population)
{
	unsigned int a = population[0].fitness, b = population[0].fitness;					//our constants

	for (int i = 0; i < GA_POPSIZE; i++) {

		a = min(population[i].fitness, a);
		b = max(population[i].fitness, b);
	}

	for (int i = 0; i < GA_POPSIZE; i++) {

		population[i].fitness = a * population[i].fitness + b;				// linear transformation
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
	int tsize = GA_TARGET.size();
	int numOfParents = 2 * (GA_POPSIZE - esize);
	int *parents = new int[numOfParents];			// the selected parents


	for (int i = esize; i < GA_POPSIZE; i++) {
		if (population[i].age > MAX_AGE) {
			ga_struct citizen;
			citizen.fitness = 0;
			citizen.age = 0;				//reset age
			citizen.str.erase();

			for (int j = 0; j < tsize; j++)
				citizen.str += (rand() % 90) + 32;
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

		//Scaling(population);			   // Linear scaling

		for (int i = 0; i < numOfParents; i++) {				   // points is a (sorted) list of																   
			*(points + i) = rand() % (totalFitness + 1);		   // random numbers from 0 to total fitness. 
		}
		sort(points, points + numOfParents);					  // sorting the array
		//parents = RWS(population,points);						  // RWS method for selecting parents
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


void mate(ga_vector &population, ga_vector &buffer)
{

	int esize = static_cast<int>(GA_POPSIZE * GA_ELITRATE);
	int tsize = GA_TARGET.size(), spos,spos2, i1, i2;
	int *parents = selectParents(population);
	elitism(population, buffer, esize);


	switch (operatorPoint) {

	case 1:						// Single Point Operator

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = parents[2 * (i - esize)];
			i2 = parents[2 * (i - esize) + 1];
			spos = rand() % tsize;

			buffer[i].str = population[i1].str.substr(0, spos) +
				population[i2].str.substr(spos, tsize - spos);

			if (rand() < GA_MUTATION) mutate(buffer[i]);
			buffer[i].age = 0;

		}

		break;

	case 2:				// Two Point Operator

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = parents[2 * (i - esize)];
			i2 = parents[2 * (i - esize) + 1];
			spos = rand() % tsize;
			spos2 = rand() % tsize;

			buffer[i].str = population[i1].str.substr(0, std::min(spos,spos2)) +
				population[i2].str.substr(std::min(spos, spos2), std::max(spos, spos2) - std::min(spos, spos2)) +
				population[i1].str.substr(std::max(spos, spos2), tsize - std::max(spos, spos2));

			if (rand() < GA_MUTATION) mutate(buffer[i]);
			buffer[i].age = 0;
		}
		break;

	case 3:				//// Uniform Point Operator

		for (int i = esize; i < GA_POPSIZE; i++) {
			i1 = parents[2 * (i - esize)];
			i2 = parents[2 * (i - esize) + 1];
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
			buffer[i].age = 0;

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
	init_pso();							// for initlizing the global , local and velocity randomly

	int tsize = GA_TARGET.size();

	Particle global_particle;			// for calculating the fitness of the global particle

	for (int k = 0; k < GA_MAXITER; k++)
	{
		if (global_particle.calc_fitness_particle(globalBest) == 0) break;		//our termination criterion 

		for (int i = 0; i < GA_POPSIZE; i++) {

			string myVelocity;
			string myStr;

			myVelocity.erase();
			myStr.erase();

			for (int j = 0; j < tsize; j++) {						// implementation the algorithm 
																   // like described in the class

				double r1 = (double)rand() / (RAND_MAX);
				double r2 = (double)rand() / (RAND_MAX);
				myVelocity += W * particle_vector[i].get_velocity()[j]
					+ C1 * r1 * (particle_vector[i].get_localBest()[j] - particle_vector[i].get_str()[j])
					+ C2 * r2 * (globalBest[j] - particle_vector[i].get_str()[j]);

				myStr += particle_vector[i].get_str()[j] + myVelocity[j]; 

			}

			particle_vector[i].set_velocity(myVelocity);					// updating the velocity and the fitness
			particle_vector[i].set_str(myStr);
			particle_vector[i].set_fitness(particle_vector[i].calc_fitness_particle(myStr));


			if (particle_vector[i].get_fitness()							// updating the local and global best
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

		cout << "Best: " << globalBest << " (" << global_particle.calc_fitness_particle(globalBest) << ")" << endl;

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
		try {									     // checking if the input is valid

			cin >> operatorInput;

			if (!cin)
			{
				cin.clear();						// reset failbit
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
			continue;									// go again
		}

	}

	operatorPoint = int(operatorInput);

	return operatorPoint;
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
		int operatorPoint;
		init_population(pop_alpha, pop_beta);
		population = &pop_alpha;
		buffer = &pop_beta;
		operatorPoint = checkInputOperator();		 		// choose which operator to use
		for (int i = 0; i < GA_MAXITER; i++) {

			clock_t begin = std::clock();		// for clock ticks
			calc_fitness(*population);		// calculate fitness using the given heuristic
			sort_by_fitness<vector<ga_struct>,ga_struct>(*population);	// sort them
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

	case 2:									 // PSO algorithm
		init_global_best();					//initialize global best
		PSO();
		//sort_by_fitness<vector<Particle>,Particle>(particle_vector);
		break;
	}

	
	const sec duration = clock::now() - before;
	std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;

	getchar();
	getchar();

	return 0;
}
