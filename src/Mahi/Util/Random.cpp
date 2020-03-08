#include <Mahi/Util/Random.hpp>
#include <cassert>
#include <ctime>
#include <random>

namespace mahi {
namespace util {
namespace {

// Random generator engine (Multiply With Carry)
// Many thanks to Volkard Henkel for the implementation.
class RandomEngine {
public:
    // Type definition for usage inside Std.Random
    typedef unsigned int result_type;

public:
    // Constructor
    explicit RandomEngine(unsigned int seedVal) { seed(seedVal); }

    // Return random number
    unsigned int operator()() {
        const std::size_t a = 1967773755;

        x = a * (x & 0xffffffff) + (x >> 32);
        return static_cast<unsigned int>(x);
    }

    // set seed (compliant to Std.Random)
    void seed(unsigned int seedVal = 0) { x = seedVal + !seedVal; }

    // Return minimal value (compliant to Std.Random)
    static constexpr unsigned int min() { return 0; }

    // Return maximal value (compliant to Std.Random)
    static constexpr unsigned int max() { return 0xffffffff; }

private:
    std::size_t x;
};

// Function initializing the engine and its seed at startup time
RandomEngine createInitialEngine() {
    return RandomEngine(static_cast<unsigned long>(std::time(nullptr)));
}

// Pseudo random number generator engine
RandomEngine g_rnEngine = createInitialEngine();

}  // namespace

int random_range(int min, int max) {
    assert(min <= max);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(g_rnEngine);
}

unsigned int random_range(unsigned int min, unsigned int max) {
    assert(min <= max);
    std::uniform_int_distribution<unsigned int> distribution(min, max);
    return distribution(g_rnEngine);
}

double random_range(double min, double max) {
    assert(min <= max);
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(g_rnEngine);
}

void set_random_seed(unsigned long seed) {
    g_rnEngine.seed(seed);
}

} // namespace util
} // namespace mahi
