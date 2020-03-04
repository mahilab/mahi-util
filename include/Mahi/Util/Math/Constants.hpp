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

namespace mahi {
namespace util {

//==============================================================================
// GENERIC
//==============================================================================

extern const double PI;     ///< pi
extern const double HALFPI; ///< pi/2
extern const double TWOPI;  ///< 2*pi
extern const double E;      ///< e
extern const double SQRT2;  /// sqrt(2)
extern const double SQRT3;  /// sqrt(3)
extern const double EPS;    ///< smallest double such that 1.0 + EPS != 1.0
extern const double INF;    ///< positive infinity (negate for negative infinity)
extern const double NaN;    ///< not-a-number

//==============================================================================
// CONVERSIONS
//==============================================================================

extern const double DEG2RAD;     ///< multiply degrees by this value to convert to radians
extern const double RAD2DEG;     ///< multiply radians by this value to convert to degrees
extern const double INCH2METER;  ///< multiply inches by this value to convert to meters
extern const double METER2INCH;  ///< multiply meters by this value to convert to inches

} // namespace util
} // namespace mahi
