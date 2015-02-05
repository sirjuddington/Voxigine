
#include "Main.h"
#include "Random.h"
#include <random>

CVAR(Int, random_seed, 0, CVAR_SAVE)

namespace Random
{
	std::mt19937	engine;
}

void Random::init()
{
	engine.seed(random_seed);
}

uint32_t Random::generateUnsigned(uint32_t min, uint32_t max)
{
	std::uniform_int_distribution<uint32_t> generator(min, max);
	return generator(engine);
}

vector<uint32_t> Random::generateUnsignedArray(unsigned num, uint32_t min, uint32_t max)
{
	vector<uint32_t> values;
	std::uniform_int_distribution<uint32_t> generator(min, max);
	for (unsigned a = 0; a < num; a++)
		values.push_back(generator(engine));
	return values;
}

int32_t Random::generateInt(int32_t min, int32_t max)
{
	std::uniform_int_distribution<int32_t> generator(min, max);
	return generator(engine);
}
