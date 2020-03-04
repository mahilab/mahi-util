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
// Author(s): Craig McDonald (craig.g.mcdonald@gmail.com)

#pragma once

#include <vector>

namespace mahi {
namespace util {

/// Direct Form II Filter
class Filter {
public:
    /// Construct Filter from transfer function coefficients
    Filter(const std::vector<double>& b,
           const std::vector<double>& a,
           unsigned int seeding = 0);

    /// Applies the filter operation for one time step
    double update(const double x);

    /// Returns the filtered value since the last update
    double get_value() const;

    /// Sets the internal states s_ to all be zero
    void reset();

    /// Returns the Filter numerator coefficients
    const std::vector<double>& get_b() const;

    /// Returns the Filter denominator coefficients
    const std::vector<double>& get_a() const;

    /// Set the Filter seeding
    void set_seeding(unsigned int seeding);

    /// Sets the Filter coefficients
    void set_coefficients(const std::vector<double>& b,
                          const std::vector<double>& a);

protected:
    /// Construct empty Filter of order n
    Filter(std::size_t n, unsigned int seeding);

private:
    /// Direct form II transposed filter implementation
    double dir_form_ii_t(const double x);

    /// Calls the Filter multiple times on the initial value to avoid startup
    /// transients
    void seed(const double x, const unsigned int iterations);

private:
    double value_;             ///< the filtered value
    std::size_t n_;            ///< filter order
    std::vector<double> b_;    ///< numerator coefficients
    std::vector<double> a_;    ///< denominator coefficients
    std::vector<double> s_;    ///< internal memory
    bool has_seeding_;         ///< indicates whether or not to call seed on first update
    bool first_update_;        ///< indicates first update upon reset
    bool will_filter_;         ///< will the coefficients actually filter (i.e a and b are not both {1,0})?
    unsigned int seed_count_;  ///< number of iterations to call on update upon seeding
};

} // namespace util
} // namespace mahi
