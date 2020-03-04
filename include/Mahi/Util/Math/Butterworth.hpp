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

#include <Mahi/Util/Math/Filter.hpp>
#include <Mahi/Util/Timing/Frequency.hpp>

namespace mahi {
namespace util {

/// A digital Butterworth filter implementation
class Butterworth : public Filter {
public:
    enum Type {
        Lowpass,  ///< Lowpass filter
        Highpass  ///< Highpass filter
    };

public:

    /// Default constructor (does not filter)
    Butterworth();

    /// Designs an n-th order lowpass or highpass digital Butterworth filter
    /// with normalized cutoff frequency Wn
    Butterworth(std::size_t n,
                double Wn,
                Type type      = Lowpass,
                unsigned int seeding = 0);

    /// Designs an n-th order lowpass or highpass digital Butterworth filter
    /// with specified cutoff and sample frequencies
    Butterworth(std::size_t n,
                Frequency cutoff,
                Frequency sample,
                Type type      = Lowpass,
                unsigned int seeding = 0);

    /// Configures an n-th order lowpass or highpass digital Butterworth filter
    /// with normalized cutoff frequency Wn
    void configure(std::size_t n, double Wn, Type type = Lowpass, unsigned int seeding = 0);


    /// Configures an n-th order lowpass or highpass digital Butterworth filter
    /// with specified cutoff and sample frequencies
    void configure(std::size_t n, Frequency cutoff, Frequency sample, Type type = Lowpass, unsigned int seeding = 0);

};

} // namespace util
} // namespace mahi

