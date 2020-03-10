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

#include <Mahi/Util/Logging/Detail/StreamMeta.hpp>
#include <Mahi/Util/StlStreams.hpp>
#include <Mahi/Util/Console.hpp>
#include <Mahi/Util/System.hpp>
#include <Mahi/Util/Timing/Timestamp.hpp>
#include <sys/stat.h>

namespace mahi {
namespace util {

/// Represents a logging severity level
enum Severity {
    None    = 0,  ///< always written
    Fatal   = 1,  ///< error that forces application abort
    Error   = 2,  ///< error that is fatal to operation, but not application
    Warning = 3,  ///< error that may cause issues, but has been accounted for
    Info    = 4,  ///< useful information needed during normal operation
    Verbose = 5,  ///< useful information not needed during normal operation
    Debug   = 6,  ///< useful information needed for diagnostics
};

inline const char* severity_to_string(Severity severity) {
    switch (severity) {
        case Fatal:
            return "FATAL";
        case Error:
            return "ERROR";
        case Warning:
            return "WARN";
        case Info:
            return "INFO";
        case Verbose:
            return "VERB";
        case Debug:
            return "DEBUG";
        default:
            return "NONE";
    }
}

inline Severity string_to_severity(const char* str) {
    for (Severity severity = Fatal; severity <= Debug;
         severity          = static_cast<Severity>(severity + 1)) {
        if (severity_to_string(severity)[0] == str[0]) {
            return severity;
        }
    }

    return None;
}

/// Encapsulates a Log record
class LogRecord {
public:
    /// Constructor
    LogRecord(Severity severity,
           const char* func,
           size_t line,
           const char* file,
           Timestamp timestamp = Timestamp());

    /// Destructor
    virtual ~LogRecord();

    // Stream operator overloads
    LogRecord &operator<<(char data);

    LogRecord &operator<<(std::ostream &(*data)(std::ostream &));

    template <typename T>
    LogRecord& operator<<(const T& data) {
        using namespace detail;
        message_ << data;
        return *this;
    }

    /// Gets the message contained by a Record
    virtual const char* get_message() const;

    /// Returns timestamp at which Record was constructed
    virtual const Timestamp& get_timestamp() const;

    /// Gets the severity of a record
    virtual Severity get_severity() const;

    /// Gets ID of thread a Record was made on
    virtual unsigned int get_tid_() const;

    /// Gets the line number where the Record was made
    virtual size_t get_line() const;

    /// Gets the name of the function in which the Record was made
    virtual const char* get_func() const;

    /// Gets the name of the file in which the Record was made
    virtual const char* get_file() const;

private:
    Timestamp timestamp_;              ///< timestamp
    const Severity severity_;          ///< Record severity
    const unsigned int tid_;           ///< thread ID
    const size_t line_;                ///< line number
    std::ostringstream message_;       ///< string stream for message
    const char* const func_;           ///< function name string
    const char* const file_;           ///< file name string
    mutable std::string func_str_;     ///< function name string
    mutable std::string message_str_;  ///< message string
};

inline std::string process_function_name(const char* func);

}  // namespace util
}  // namespace mahi