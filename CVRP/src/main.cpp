#include <iostream>					// for cout etc.
#include <time.h>					// for random seed
#include <chrono>					// for elapsed time
#include <ctime>					// for clock ticks



using namespace std;				// polluting global namespace, but hey...


int main()
{
	
	using clock = std::chrono::system_clock;
	using sec = std::chrono::duration<double>;
	const auto before = clock::now();				// for elapsed time
	srand(unsigned(time(NULL)));
	clock_t begin = std::clock();		// for clock ticks
	clock_t end = std::clock();
	float time_spent = (float)(end - begin) / CLOCKS_PER_SEC;
	std::cout << "Clock Ticks: " << time_spent << "s" << std::endl;
	const sec duration = clock::now() - before;
	std::cout << "Time Elapsed: " << duration.count() << "s" << std::endl;
	getchar();
	return 0;
}

