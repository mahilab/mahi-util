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

#include <cmath>
#include <complex>
#include <vector>
#include <memory>
#include <cassert>

namespace mahi {
namespace util {

//==============================================================================
// USINGS
//==============================================================================

using std::abs;
using std::fmod;
using std::remainder;

using std::exp;
using std::log;
using std::log10;
using std::log2;

using std::pow;
using std::sqrt;
using std::cbrt;
using std::hypot;

using std::sin;
using std::cos;
using std::tan;
using std::asin;
using std::acos;
using std::atan;
using std::atan2;

using std::sinh;
using std::cosh;
using std::tanh;
using std::asinh;
using std::acosh;
using std::atanh;

using std::ceil;
using std::floor;
using std::trunc;
using std::round;

//==============================================================================
// GENERIC FUNCTIONS
//==============================================================================

/// Returns -1 for negative numbers, 1 for positive numbers, and 0 for numbers equal to 0.
template <typename T>
inline int sign(T val);

/// Returns true if an integer is even
inline bool isEven(int value);

/// Returns true if an integer is odd
inline bool isOdd(int value);

/// Clamps a float between min and max
template <typename T>
inline T clamp(T value, T min, T max);

/// Clamps value between -abs_max and +abs_max
template <typename T>
inline T clamp(T value, T abs_max);

/// Clamps a float between 0 and 1
template <typename T>
inline T clamp01(T value);

/// Wraps an angle in radians to within [-PI, PI)
template <typename T>
inline T wrap_to_pi(T angle);

/// Wraps an angle in radians to within [0, 2*PI)
template <typename T>
inline T wrap_to_2pi(T angle);

/// Determines if two floats a and b are approximately equal
template <typename T>
inline bool approximately(T a, T b, T tol = EPS);

/// Interpolate values
template <typename T>
inline T interp(T x, T x0, T x1, T y0, T y1);

/// Determine order of magnitude of number
template <typename T>
inline int order_of_mag(T value);

/// Precison
inline std::size_t precision(int order);

/// Rounds a float up to the nearest interval
template <typename T>
inline T round_up_nearest(T value, T interval);

/// Rounds a float down to the nearest interval
template <typename T>
inline T round_down_nearest(T value, T interval);

/// Rounds a float up or down to the nearest interval
template <typename T>
inline T round_nearest(T value, T interval);

/// Returns minimum value in a vector
template <typename T, typename TArray>
T min_element(const TArray& values);

/// Returns maximum value in a vector
template <typename T, typename TArray>
T max_element(const TArray& values);

/// Computes a proportional-derivative control effort given gains, referenc state, and current state
template <typename T>
inline T pd_controller(T kp,T kd,T x_ref,T x,T xd_ref,T xd);

/// Logistic sigmoid
template <typename T>
inline T sigmoid(T a);

//==============================================================================
// STATISTICS
//==============================================================================

/// Creates an evenly space array of N values between a and b
template <class Container, typename R>
inline void linspace(R a, R b, Container& array);

/// Computes the sum of a vector of floats
template <class Container>
inline auto sum(const Container& data);

/// Computes the mean of a vector of floats
template <class Container>
inline auto mean(const Container& c);

/// Returns the population standard deviation of a vector of data
template <class Container>
inline auto stddev_p(const Container& data);

/// Returns the sample standard deviation of a vector of data
template <class Container>
inline auto stddev_s(const Container& data);

/// Computes a linear regression slope and intercept {m, b} for y = m*x + b
template <class ContainerX, class ContainerY, typename T>
inline void linear_regression(const ContainerX& x, const ContainerY& y, T& mOut, T& bOut);

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

#include <Mahi/Util/Math/Detail/Functions.inl>
