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
//            Craig McDonald (craig.g.mcdonald@gmail.com)

#pragma once

#include <complex>
#include <vector>

namespace mahi {
namespace util {

//==============================================================================
// GENERIC FUNCTIONS
//==============================================================================

/// Returns -1 for negative numbers, 1 for positive numbers, and 0 for numbers
/// equal to 0.
template <typename T>
int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

/// Returns minimum value in a vector
extern double min(const std::vector<double>& values);

/// Returns maximum value in a vector
extern double max(const std::vector<double>& values);

/// Clamps value between min and max
extern double saturate(double value, double min, double max);

/// Clamps value between -abs_max and +abs_max
extern double saturate(double value, double abs_max);

/// Returns true if a and b are approximately equal to each other within a
/// tolerance (machine precision by default)
extern bool approx_equal(double a, double b, double tolerance);

/// Returns a linearly spaced vector with #n elements between #a and #b.
extern std::vector<double> linspace(double a, double b, std::size_t n);

/// Computes a proportional-derivative control effort given gains, reference
/// state, and current state
extern double pd_controller(double kp,
                            double kd,
                            double x_ref,
                            double x,
                            double xd_ref,
                            double xd);

/// Logistic sigmoid
extern double sigmoid(double a);

/// Computes the automatic derivative of a function.
extern double auto_diff(std::complex<double> (*f)(std::complex<double>),
                        double x);

/// Wraps an angle in radians to the interval [0 2*PI]
extern double wrap_to_2pi(double radians);

/// Wraps an angle in radians to the interval [-pi pi]
extern double wrap_to_pi(double radians);

//==============================================================================
// STATISTICS
//==============================================================================

/// Returns the absolute value of a vector
extern std::vector<double> abs_vec(const std::vector<double>& data);

/// Returns the sum of a vector of data
extern double sum(const std::vector<double>& data);

/// Returns the mean of a vector of data
extern double mean(const std::vector<double>& data);

/// Returns the population standard deviation of a vector of data
extern double stddev_p(const std::vector<double>& data);

/// Returns the sample standard deviation of a vector of data
extern double stddev_s(const std::vector<double>& data);

/// Computes a linear regression slope and intercept {m, b} for y = m*x + b
extern std::vector<double> linear_regression(const std::vector<double>& x,
                                             const std::vector<double>& y);

/// Computes the sample mean and covariance for a multivariate gaussian
/// distribution, where the second dimension of sample_data (cols) correspond to
/// random variables and the first dimension of sample data (rows) corresponds
/// to observations
extern void gauss_mlt_params(
    const std::vector<std::vector<double>>& sample_data,
    std::vector<double>& sample_mean,
    std::vector<std::vector<double>>& sample_cov);

}  // namespace util
}  // namespace mahi
