#include <random>
#include "mpuint.h"
#include "random.h"

void Random(mpuint& x)
{
    std::random_device rd; // obtain a random number from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> distr(0, 255); // define the range

    for (unsigned i = 0; i < x.length; i++)
        x.value[i] = distr(gen) << 8 | distr(gen);
}