#pragma once

namespace mahi {
namespace util {

/// Returns an int random number in the interval [min, max].
int random_range(int min, int max);

/// Returns an unsigned int random number in the interval [min, max].
unsigned int random_range(unsigned int min, unsigned int max);

/// Returns a double random number in the interval [min, max].
double random_range(double min, double max);

/// Sets the seed of the random number generator. Without calling this function, the seed is different at each program startup.
/// Setting the seed manually is useful when you want to reproduce a given sequence of random numbers. 
void set_random_seed(unsigned long seed);

} // namespace util
} // namespace mahi
