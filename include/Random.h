#ifndef RANDOM_H
#define RANDOM_H

#include <random>

namespace Domigon
{
	/* This class allow to simply generate random numbers that are between two intervals */
	class Random {
	public:
		// Constructor (min and max are inclusives)
		Random(int min, int max);

		// Destructor
		~Random();

		// Generate a random number
		unsigned short rand();

		/*****************************************************************************************************************************/

	private:
		// Create a random seed for the mersenne twister
		std::random_device m_rd;

		// Pseudo-random generator
		std::mt19937 m_mt;

		// Random number distribution
		std::uniform_int_distribution<unsigned short> m_dist;
	};
}

#endif
