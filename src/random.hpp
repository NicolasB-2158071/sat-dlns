#pragma once

#include <random>

class Random
{
public:
    static int get_uniform(int min, int max);

private:
    static std::random_device m_rd;
    static std::mt19937 m_generator;
};