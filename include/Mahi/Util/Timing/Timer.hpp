// MIT License
//
// Copyright (c) 2020 Mechatronics and Haptic Interfaces Lab - Rice University
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <Mahi/Util/Timing/Clock.hpp>
#include <Mahi/Util/Timing/Frequency.hpp>

namespace mahi {
namespace util {

/// CPU based fixed rate waitable timer.
class Timer {
public:
    /// The waiting mode to be used when wait() is called.
    ///
    /// The WaitMode should be chosen wisely depending on the Timer period and
    /// the target platform. On Windows, the smallest amount of time a thread
    /// can be put to sleep is around 1 ms. Therefore, Sleep and Hybrid
    /// shouldn't be used on Windows for Timers running at 1000 Hz or higher;
    /// use Busy. On real-time Linux, threads can sleep for much smaller
    /// periods, so Sleep and Hybrid can be used reliably. Generally, using
    /// Hybrid over Sleep will be more accurate since Sleep can go over the
    /// requested sleep period.
    enum WaitMode {
        Busy,     ///< Waits 100% remaining time using a busy while loop
        Sleep,    ///< Waits 100% remaining time by sleeping the thread
        Hybrid    ///< Waits x% remaining time using Sleep, then 100-x% using Busy (default x = 90%, set with set_hybrid_percentage())
    };

public:
    /// Constructs Timer from frequency. Starts the Timer on construction.
    Timer(Frequency frequency, WaitMode mode = WaitMode::Busy, bool emit_warnings = true);

    /// Constructs Timer from wait period. Starts the Timer on construction.
    Timer(Time period, WaitMode mode = WaitMode::Busy, bool emit_warnings = true);

    /// Destructor
    ~Timer();

    /// Sets the Timer wait mode
    void set_wait_mode(WaitMode mode);

    /// Restarts the Timer and returns the elapsed time
    Time restart();

    /// Waits the Timer and returns the actual elapsed time after the wait completes
    Time wait();

    /// Gets the actual elapsed time since construction or last call to restart().
    Time get_elapsed_time() const;

    /// Gets the ideal elapsed time since construction or last call to
    /// restart(). Equal to the tick count times the Timer period
    Time get_elapsed_time_ideal() const;

    /// Gets the elapsed number of ticks since construction or the last call to restart().
    int64 get_elapsed_ticks() const;

    /// Get the number of times the Timer has missed a tick deadline
    int64 get_misses() const;

    /// Gets the deadline miss rate of the Timer
    double get_miss_rate() const;

    /// Gets the Timer frequency
    Frequency get_frequency() const;

    /// Gets the Timer period
    Time get_period() const;

    /// Gets the ratio of time waited to time elapsed for benchmarking purposes
    double get_wait_ratio() const;

    /// Sets the acceptable miss rate of the Timer before Warning are logged (default 0.01 = 1%)
    void set_acceptable_miss_rate(double rate);

    /// Set the percentage of time to sleep for Hybrid timers (default = 0.9)
    void set_hybrid_percentage(double sleep_percent);

    /// Enable deadline miss warnings
    void enable_warnings();

    /// Disable deadline miss warnings
    void disable_warnings();

protected:
    WaitMode mode_;      ///< the Timer's waiting mode
    Clock clock_;        ///< the Timer's internal clock
    Time period_;        ///< the Timer's waiting period
    int64 ticks_;        ///< the running tick count
    int64 misses_;       ///< number of misses
    Time prev_time_;     ///< time saved at previous call to wait or restart
    Time waited_;        ///< accumulated wait time
    double rate_;        ///< acceptable miss rate
    double hybrid_perc_; ///< percentage of time slept in hybrid mode (default = 0.9)
    bool warnings_;      ///< emit warnings?
};

} // namespace util
} // namespace mahi