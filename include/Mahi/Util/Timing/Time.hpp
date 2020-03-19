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

#include <Mahi/Util/Types.hpp>
#include <ostream>

namespace mahi {
namespace util {

class Frequency;

/// Represents a time value.
class Time {
public:
    /// Default constructor. Sets time value to zero. To construct valued time
    /// objects, use mahi::util::seconds, mahi::util::milliseconds or mahi::util::microseconds.
    Time();

    /// Overloads stream operator
    friend std::ostream& operator<<(std::ostream& os, const Time& t);

    /// Return the time value as a number of seconds.
    double as_seconds() const;

    /// Return the time value as a number of milliseconds.
    int32 as_milliseconds() const;

    /// Return the time value as a number of microseconds.
    int64 as_microseconds() const;

    /// Returns the reciprocal time as a Frequency
    Frequency to_frequency() const;

public:
    static const Time Zero;  ///< Predefined "zero" time value
    static const Time Inf;   ///< Predefined "infinite time value

private:
    friend Time seconds(double);
    friend Time milliseconds(int32);
    friend Time microseconds(int64);

    /// Internal constructor from a number of microseconds.
    explicit Time(int64 microseconds);

private:
    int64 microseconds_;  ///< Time value stored as microseconds
};

//==============================================================================
// INSTANTIATION FUNCTIONS
//==============================================================================

/// Construct a time value from a number of seconds
Time seconds(double amount);

/// Construct a time value from a number of milliseconds
Time milliseconds(int32 amount);

/// Construct a time value from a number of microseconds
Time microseconds(int64 amount);

//==============================================================================
// USER DEFINED LITERALS
//==============================================================================

/// Construct a time value from a number of seconds ( e.g Time x = 1.2_s; )
Time operator ""_s(long double ammount);

/// Construct a time value from a number of seconds ( e.g Time x = 12_s; )
Time operator ""_s(unsigned long long int ammount);

/// Construct a time value from a number of milliseconds ( e.g Time x = 15_ms; )
Time operator ""_ms(unsigned long long int ammount);

/// Construct a time value from a number of microseconds ( e.g Time x = 100_us; )
Time operator ""_us(unsigned long long int ammount);

//==============================================================================
// OPERATOR OVERLOADS
//==============================================================================

/// Overload of << stream operator
std::ostream& operator<<(std::ostream& os, const Time& t);

/// Overload of == operator to compare if two time values are equal
bool operator==(Time left, Time right);

/// Overload of != operator to compare if two time values are not equal
bool operator!=(Time left, Time right);

/// Overload of < operator to compare if left time is less than right
bool operator<(Time left, Time right);

/// Overload of > operator to compare if right time is greater than right
bool operator>(Time left, Time right);

/// Overload of <= operator to compare if left time is less or equal than right
bool operator<=(Time left, Time right);

/// Overload of >= operator to compare if left time is greater or equal than
/// right
bool operator>=(Time left, Time right);

/// Overload of unary - operator to negate a time value
Time operator-(Time right);

/// Overload of binary + operator to add two time values
Time operator+(Time left, Time right);

/// Overload of binary += operator to add/assign two time values
Time& operator+=(Time& left, Time right);

/// Overload of binary - operator to subtract two time values
Time operator-(Time left, Time right);

/// Overload of binary -= operator to subtract/assign two time values
Time& operator-=(Time& left, Time right);

/// Overload of binary * operator to scale a time value
Time operator*(Time left, double right);

/// Overload of binary * operator to scale a time value
Time operator*(Time left, int64 right);

/// Overload of binary * operator to scale a time value
Time operator*(double left, Time right);

/// Overload of binary * operator to scale a time value
Time operator*(int64 left, Time right);

/// Overload of binary *= operator to scale/assign a time value
Time& operator*=(Time& left, double right);

/// Overload of binary *= operator to scale/assign a time value
Time& operator*=(Time& left, int64 right);

/// Overload of binary / operator to scale a time value
Time operator/(Time left, double right);

/// Overload of binary / operator to scale a time value
Time operator/(Time left, int64 right);

/// Overload of binary /= operator to scale/assign a time value
Time& operator/=(Time& left, double right);

/// Overload of binary /= operator to scale/assign a time value
Time& operator/=(Time& left, int64 right);

/// Overload of binary / operator to compute the ratio of two time values
double operator/(Time left, Time right);

/// Overload of binary % operator to compute remainder of a time value
Time operator%(Time left, Time right);

/// Overload of binary %= operator to compute/assign remainder of a time value
Time& operator%=(Time& left, Time right);

} // namespace util
} // namespace mahi

