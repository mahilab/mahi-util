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

#include <Mahi/Util/NonCopyable.hpp>
#include <string>

namespace mahi {
namespace util {

/// Abstract base class from which all physical devices derive.
class Device : public NonCopyable {
public:
    /// Default constructor.
    Device();
    /// Constructs a Device with a specific string name
    Device(const std::string& name);
    /// Default destructor.
    virtual ~Device();
    /// Opens communication with Deviceand returns TRUE if open successful, FALSE otherwise
    bool open();
    /// Closes communication with Deviceand returns TRUE if open successful, FALSE otherwise
    bool close();
    /// Returns whether the DAQ is open or closed returns TRUE if open, FALSE if closed
    bool is_open() const;
    /// Enables the Device and returns TRUE if successful, FALSE otherwise
    bool enable();
    /// Disables the Device and returns TRUE if successful, FALSE otherwise
    bool disable();
    /// Returns TRUE if the Device is enabled or FALSE if it is disabled
    bool is_enabled() const;
    /// Gets the Device's string name
    const std::string& name() const;

protected:
    /// Sets the Device's string name
    void set_name(const std::string& name);
    /// Implement this function with code that should be called when enabled
    /// return TRUE if successful, FALSE otherwise
    virtual bool on_enable();
    /// Implement this function with code that should be called when disabled
    /// return TRUE if successful, FALSE otherwise
    virtual bool on_disable();
    /// Implement this function to open communication with your Device.
    /// return TRUE if open successful, FALSE otherwise
    virtual bool on_open();
    /// Implement this function to close communication with your Device.
    /// return TRUE if close successful, FALSE otherwise
    virtual bool on_close();

private:
    bool        enabled_;  ///< The Device enabled status
    bool        open_;     ///< The Device open status
    std::string name_;     ///< The Device name
};

}  // namespace util
}  // namespace mahi