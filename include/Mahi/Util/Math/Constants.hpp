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
#include <limits>
#include <cmath>

namespace mahi {
namespace util {

//==============================================================================
// GENERIC
//==============================================================================

constexpr double PI     = 3.141592653589793238462643383279502884;
constexpr double HALFPI = PI * 0.5;
constexpr double TWOPI  = 2 * PI;
constexpr double E      = 2.718281828459045235360287471352662497;
constexpr double SQRT2  = 1.414213562373095048801688724209698078;
constexpr double SQRT3  = 1.7320508075688772935274463415058723669;
constexpr double EPS    = std::numeric_limits<double>::epsilon();
constexpr double INF    = std::numeric_limits<double>::infinity();
constexpr double NaN    = std::numeric_limits<double>::quiet_NaN();

//==============================================================================
// CONVERSIONS
//==============================================================================

constexpr double DEG2RAD    = PI / 180;
constexpr double RAD2DEG    = 180 / PI;
constexpr double INCH2METER = 0.0254;
constexpr double METER2INCH = 1.0 / INCH2METER;

//==============================================================================
// MISC
//==============================================================================

constexpr double G = 9.80665; // [m/s^2]

} // namespace util
} // namespace mahi
