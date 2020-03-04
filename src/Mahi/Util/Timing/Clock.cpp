#include <Mahi/Util/Timing/Clock.hpp>

#ifdef _WIN32
#include <windows.h>
#elif __APPLE__
#include <mach/mach_time.h>
#else
#include <time.h>
#endif

namespace mahi {
namespace util {

//==============================================================================
// CLASS DEFINITIONS
//==============================================================================

Clock::Clock() :
    start_time_(Clock::get_current_time())
{
}

Time Clock::get_elapsed_time() const {
    return Clock::get_current_time() - start_time_;
}

Time Clock::restart() {
    Time now = Clock::get_current_time();
    Time elapsed = now - start_time_;
    start_time_ = now;
    return elapsed;
}

//==============================================================================
// WINDOWS IMPLEMENTATION
//==============================================================================

#ifdef _WIN32

LARGE_INTEGER get_frequency() {
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return frequency;
}

Time Clock::get_current_time() {
    // Get the frequency of the performance counter
    // (it is constant across the program lifetime)
    static LARGE_INTEGER frequency = get_frequency();
    LARGE_INTEGER time;
   // Get the current time
    QueryPerformanceCounter(&time);
    // Return the current time as microseconds
    return mahi::util::microseconds(1000000 * time.QuadPart / frequency.QuadPart);
}

#elif __APPLE__

//==============================================================================
// APPLE IMPLEMENTATION
//==============================================================================

Time Clock::get_current_time() {
    static mach_timebase_info_data_t frequency = {0, 0};
    if (frequency.denom == 0)
        mach_timebase_info(&frequency);
    uint64 nanoseconds = mach_absolute_time() * frequency.numer / frequency.denom;
    return mahi::util::microseconds(nanoseconds / 1000);
}

#else

//==============================================================================
// LINUX IMPLEMENTATION
//==============================================================================

Time Clock::get_current_time() {
    // POSIX implementation
    // https://linux.die.net/man/3/clock_gettime
    // https://forums.ni.com/t5/NI-Linux-Real-Time-Discussions/Help-to-solve-a-problem-with-C-on-cRIO-9068/td-p/3469892
    timespec time;
    clock_gettime(CLOCK_MONOTONIC_RAW, &time);
    return mahi::util::microseconds(static_cast<uint64>(time.tv_sec) * 1000000 + time.tv_nsec / 1000);
}

#endif

} // namespace util
} // namespace mahi


