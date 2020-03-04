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

namespace mahi {
namespace util {

//==============================================================================
// CLASS DECLARATION
//==============================================================================

class Writer {
public:

    Writer(Severity max_severity = Debug) : max_severity_(max_severity) {}

    virtual ~Writer() {}

    virtual void write(const LogRecord& record) = 0;

    Severity get_max_severity() const { return max_severity_; }

    virtual void set_max_severity(Severity severity) { max_severity_ = severity; }

    bool check_severity(Severity severity) const {
        return severity <= max_severity_;
    }

protected:

    Severity max_severity_;
};
} // namespace util
} // namespace mahi

//==============================================================================
// CLASS DOCUMENTATION
//==============================================================================
