#include "Random.h"

namespace Domigon
{
	Random::Random(int min, int max) : m_rd {}, m_mt{ m_rd() }, m_dist{ min, max } {}

	Random::~Random() {}

	/*****************************************************************************************************************************/

	unsigned short Random::rand()
	{
		return m_dist(m_mt);
	}

}
