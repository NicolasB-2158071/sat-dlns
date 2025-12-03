#include "Random.hpp"

std::random_device Random::m_rd;
std::mt19937 Random::m_generator{Random::m_rd()};

int Random::get_uniform(int min, int max)
{
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(m_generator);
}