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
#include <Mahi/Util/Logging/Writers/Writer.hpp>
#include <Mahi/Util/Concurrency/Mutex.hpp>
#include <Mahi/Util/Console.hpp>
#include <fmt/format.h>

namespace mahi {
namespace util {

//==============================================================================
// CLASS DECLARATION
//==============================================================================
//
template <class Formatter>
class ConsoleWriter : public Writer {
public:

    /// Default constructor
    ConsoleWriter(Severity max_severity = Debug) : Writer(max_severity) {}

    /// Formats then writers a Record to the console
    virtual void write(const LogRecord& record) override {
        std::string str = Formatter::format(record);
        Lock lock(mutex_);
        fmt::print("{}",str);
    }

protected:
    Mutex mutex_;
};

} // namespace util
} // namespace mahi

//==============================================================================
// CLASS DOCUMENTATION
//==============================================================================
