#include "randomgenerators.h"

#include <random>

static std::mt19937 rng(std::random_device {}());

size_t randint(size_t min, size_t max)
{
    std::uniform_int_distribution<size_t> dist(min, max);
    return dist(rng);
}

double randomUniform(double min, double max)
{
    std::uniform_real_distribution<> dist(min, max);
    return dist(rng);
}

double randomNormal(double mean, double std, double minVal, double maxVal)
{
    std::normal_distribution<double> dist(mean, std);
    return dist(rng);
}

double randomExponential(double lambda)
{
    std::exponential_distribution<> dist(lambda);
    return dist(rng);
}
