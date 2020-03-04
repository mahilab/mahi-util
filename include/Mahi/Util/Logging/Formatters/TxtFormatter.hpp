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

#include <Mahi/Util/Logging/Detail/LogUtil.hpp>
#include <iomanip>

namespace mahi {
namespace util {

//==============================================================================
// CLASS DECLARATION
//==============================================================================
//
class TxtFormatter {
public:
    static std::string header() { return std::string(); }

    static std::string format(const LogRecord& record) {
        std::ostringstream ss;
        ss << record.get_timestamp().yyyy_mm_dd_hh_mm_ss_mmm() << " ";
        ss << std::setfill(' ') << std::setw(5) << std::left
           << severity_to_string(record.get_severity()) << " ";
        ss << "[" << record.get_tid_() << "] ";
        ss << record.get_message() << "\n";
        return ss.str();
    }
};
} // namespace util
} // namespace mahi

//==============================================================================
// CLASS DOCUMENTATION
//==============================================================================
