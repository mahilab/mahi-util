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
// This particular source file includes code which has been adapted from the
// following open-source projects (all external licenses attached at bottom):
//     SFML - Simple and Fast Multimedia Library
//
// Author(s): Evan Pezent (epezent@rice.edu)

#pragma once

#include <Mahi/Util/Timing/Time.hpp>

namespace mahi {
namespace util {

class Timer;

/// Utility class that measures elapsed time.
class Clock {
public:
    /// Default constructor. Clock automatically starts on construction.
    Clock();

    /// Get the elapsed time since construction or last call to restart().
    Time get_elapsed_time() const;

    /// Restart the clock back to zero and return elapsed time since started.
    Time restart();

private:
    friend class Timer;

    /// Gets the time since epoch. Relative to nothing in particular.
    static Time get_current_time();

    Time start_time_;  ///< Time of last reset, in microseconds.
};

} // namespace util
} // namepsace mahi 