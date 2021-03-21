
#include "Particle.h"
#include "Genetic5.h"


Particle::Particle()
{
	int tsize = GA_TARGET.size();
	str.erase();
	velocity.erase();


	for (int j = 0; j < tsize; j++)
	{
		str += (rand() % 90) + 32;
		velocity += (rand() % 90) + 32;
	}

	fitness = calc_fitness_particle(str);
	localBest = str;
}


void Particle::set_str(string str)
{
	this->str = str;
}

void Particle::set_fitness(int fitness)
{
	this->fitness = fitness;
}

void Particle::set_localBest(string localBest)
{
	this->localBest = localBest;
}

void Particle::set_velocity(string velocity)
{
	this->velocity = velocity;
}


string Particle::get_str()
{
	return str;
}

int Particle::get_fitness()
{
	return fitness;
}

string Particle::get_localBest()
{
	return localBest;
}

string Particle::get_velocity()
{
	return velocity;
}


int Particle::calc_fitness_particle(string citizenStr)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;
	fitness = 0;

	switch (Genetic5::getHueristic())
	{

	case 1:
		for (int j = 0; j < tsize; j++) {
			fitness += abs(int(citizenStr[j] - target[j]));
		}
		break;

	case 2:

		fitness = tsize * 10;
		for (int j = 0; j < tsize; j++) {
			if (citizenStr[j] == target[j]) {
				fitness -= 10;
			}

			else {
				for (int k = 0; k < tsize; k++) {
					if (citizenStr[j] == target[k]) {
						fitness -= 1;
						break;
					}
				}
			}
		}
		break;

	}
		
	return fitness;

}



