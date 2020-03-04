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

#include <Mahi/Util/Concurrency/Lock.hpp>
#include <memory>

namespace mahi {
namespace util {

/// Blocks concurrent access to shared resources from multiple threads (legacy, prefer std::mutex)
class Mutex : public Lockable, NonCopyable {
public:
    /// Default constructor
    Mutex();

    /// Destructor
    ~Mutex();

    /// Lock the mutex
    void lock() override;

    /// Unlock the mutex
    void unlock() override;

private:
    class Impl;                   ///< Pimpl idiom
    std::unique_ptr<Impl> impl_;  ///< OS-specific implementation
};

} // namespace util
} // namespace mahi