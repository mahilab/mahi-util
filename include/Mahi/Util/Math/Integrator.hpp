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

#include <Mahi/Util/Timing/Time.hpp>

namespace mahi {
namespace util {

/// Integrates a continous waveform
class Integrator {
public:
    enum Technique { Trapezoidal, Simpsons };

    /// Constructor
    Integrator(double initial_value = 0.0, Technique technique = Trapezoidal);

    /// Set technique
    void set_technique(Technique technique);

    /// Integrats x with respect to time
    double update(double x, const Time& t);

    /// Resets the integrators
    void reset();

    /// Set the initial value
    void set_init(double initial_value);

    /// Gets the integrated value since the last update
    double get_value() const;

private:
    Technique technique_;  ///< Integration technique to be used
    int step_count_;       ///< Running conter of calls to integrate()
    double last_last_x_;   ///< Integrand at two previous calls to integrate()
    double last_x_;        ///< Integrand at previous call to integrate()
    Time last_last_t_;     ///< Time at two previous calls to integrate()
    Time last_t_;          ///< Time at previous call to integrate()
    double integral_;      ///< The integral value
};

} // namespace util
} // namespace mahi
