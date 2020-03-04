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
#include <string>

namespace mahi {
namespace util {

/// Encapsulates a timestamp
class Timestamp {
public:
    /// Default constructor
    Timestamp();
    
    /// Returns timestamp string as "yyyy-mm-dd"
    std::string yyyy_mm_dd() const;

    /// Returns timestamp as "hh:mm:ss.mmm"
    std::string hh_mm_ss_mmm() const;

    /// Returns timestamp as "yyyy-mm-dd_hh:mm:ss"
    std::string yyyy_mm_dd_hh_mm_ss() const;

    /// Returns timestamp as "yyyy-mm-dd_hh:mm:ss.mmm"
    std::string yyyy_mm_dd_hh_mm_ss_mmm() const;

public:
    int year;      ///< year
    int month;     ///< month                    [1-12]
    int yday;      ///< day of year              [1-366]
    int mday;      ///< day of month             [1-31]
    int wday;      ///< day of week (Sunday = 1) [1-7]
    int hour;      ///< hour                     [0-23]
    int min;       ///< minute                   [0-59]
    int sec;       ///< second                   [0-59]
    int millisec;  ///< millisecond              [0-999]
};

} // namespace util
} // namespace mahi
