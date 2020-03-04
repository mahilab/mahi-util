#include <Mahi/Util/Timing/Timer.hpp>
#include <Mahi/Util/Logging/Log.hpp>
#include <thread>

namespace mahi {
namespace util {

static void wait_busy(const Time& duration) {
    Clock temp_clock;
    while (temp_clock.get_elapsed_time() < duration) {
        // kill the CPU :)
    }
}

static void wait_sleep(const Time& duration) {
    std::this_thread::sleep_for(std::chrono::microseconds(duration.as_microseconds()));
}

Timer::Timer(Frequency frequency, WaitMode mode, bool emit_warnings) :
    Timer(frequency.to_time(), mode, emit_warnings)
{
}

Timer::Timer(Time period, WaitMode mode, bool emit_warnings) :
    mode_(mode),
    clock_(Clock()),
    period_(period),
    ticks_(0),
    misses_(0),
    prev_time_(Clock::get_current_time()),
    rate_(0.01),
    hybrid_perc_(0.9),
    warnings_(emit_warnings)
{
}

Timer::~Timer() { }

void Timer::set_wait_mode(WaitMode mode) {
    mode_ = mode;
}

Time Timer::restart() {
    ticks_  = 0;
    misses_ = 0;
    prev_time_ = Clock::get_current_time();
    waited_ = Time::Zero;
    return clock_.restart();
}

Time Timer::wait() {
    Time remaining_time = period_ - (Clock::get_current_time() - prev_time_);

    if (remaining_time < Time::Zero) {
        misses_++;
        double miss_rate = get_miss_rate();
        if (miss_rate >= rate_ && ticks_ > 1000 && warnings_) {
            LOG(Warning) << "Timer miss rate of " << miss_rate << " exceeded acceptable rate of " << rate_;
        }
    }
    else if (remaining_time > Time::Zero) {
        waited_ += remaining_time;
        if (mode_ == WaitMode::Busy)
            wait_busy(remaining_time);
        else if (mode_ == WaitMode::Sleep)
            wait_sleep(remaining_time);
        else if (mode_ == WaitMode::Hybrid) {
            wait_sleep(remaining_time * hybrid_perc_);
            remaining_time = period_ - (Clock::get_current_time() - prev_time_);
            wait_busy(remaining_time);
        }
    }
    prev_time_ = Clock::get_current_time();
    ticks_++;
    return get_elapsed_time();
}

Time Timer::get_elapsed_time() const {
    return clock_.get_elapsed_time();
}

Time Timer::get_elapsed_time_ideal() const {
    return period_ * ticks_;
}

int64 Timer::get_elapsed_ticks() const {
    return ticks_;
}

int64 Timer::get_misses() const {
    return misses_;
}

double Timer::get_miss_rate() const  {
    return static_cast<double>(misses_) / static_cast<double>(ticks_);
}

Frequency Timer::get_frequency() const {
    return period_.to_frequency();
}

Time Timer::get_period() const {
    return period_;
}

double Timer::get_wait_ratio() const {
    return waited_.as_seconds() / get_elapsed_time().as_seconds();
}

void Timer::set_acceptable_miss_rate(double rate) {
    rate_ = rate;
}

void Timer::set_hybrid_percentage(double sleep_percent) {
    hybrid_perc_ = sleep_percent;
}

void Timer::enable_warnings() {
    warnings_ = true;
}

void Timer::disable_warnings() {
    warnings_ = false;
}

} // namespace util
} // namespace mahi
