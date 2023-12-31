#include "rand.hpp"
#include <random>
#include <chrono>

namespace cgp
{


static std::default_random_engine generator(0);
static std::uniform_real_distribution<float> distribution(0,1);
static std::normal_distribution<float> distribution_normal(0, 1);

float rand_uniform(float const value_min, float const value_max)
{
    return distribution(generator)* (value_max-value_min) + value_min;
}
float rand_normal(float const average, float const stddev)
{
    return stddev * distribution_normal(generator) + average;
}

void rand_initialize_generator()
{
    unsigned seed = std::chrono::steady_clock::now().time_since_epoch().count();
    generator = std::default_random_engine(seed);
}

}
