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

#include <Mahi/Util/Logging/Formatters/TxtFormatter.hpp>
#include <Mahi/Util/Logging/Writers/ColorConsoleWriter.hpp>
#include <Mahi/Util/Logging/Writers/RollingFileWriter.hpp>
#include <Mahi/Util/Logging/Writers/Writer.hpp>
#include <Mahi/Util/Templates/Singleton.hpp>
#include <cstring>
#include <vector>

#ifndef DEFAULT_LOGGER
#define DEFAULT_LOGGER 0
#endif

/// Gets name of calling funcion
#ifdef _MSC_VER
#define LOG_GET_FUNC() __FUNCTION__
#else
#define LOG_GET_FUNC() __PRETTY_FUNCTION__
#endif

/// Can be turned on to capture file name with Record
#if MAHI_LOG_CAPTURE_FILE
#define LOG_GET_FILE() __FILE__
#else
#define LOG_GET_FILE() ""
#endif

namespace mahi {
namespace util {

//==============================================================================
// LOGGER CLASS
//==============================================================================

template <int instance>
class Logger : public Singleton<Logger<instance> >, public Writer {
public:
    /// Constructs a new Logger instance with a max severity
    Logger(Severity maxSeverity = None) : Writer(maxSeverity) {}

    /// Adds a write to the Logger
    Logger& add_writer(Writer* writer) {
        assert(writer != this);
        writers_.push_back(writer);
        return *this;
    }

    Writer& get_writer(std::size_t index) {
        return *writers_[index];
    }

    /// Writes a Record to the Logger
    virtual void write(const LogRecord& record) override {
        if (check_severity(record.get_severity())) {
            *this += record;
        }
    }

    void operator+=(const LogRecord& record) {
        for (std::vector<Writer*>::iterator it = writers_.begin();
             it != writers_.end(); ++it) {
            if ((*it)->check_severity(record.get_severity()))
                (*it)->write(record);
        }
    }

    virtual void set_max_severity(Severity severity) override {
        max_severity_ = severity;
        for (std::size_t i = 0; i < writers_.size(); ++i)
            writers_[i]->set_max_severity(severity);
    }

private:
    std::vector<Writer*> writers_;  ///< writers for this Logger
};

//==============================================================================
// CUSTOM LOGGING INITIALIZER FUNCTIONS
//==============================================================================

/// Gets a logger instance
template <int instance>
inline Logger<instance>* get_logger() {
    return Logger<instance>::get_instance();
}

/// Initializes a logger with a custom Writer (specific logger instance)
template <int instance>
inline Logger<instance>& init_logger(Severity max_severity, Writer* writer) {
    static Logger<instance> logger(max_severity);
    logger.add_writer(writer);
    logger += LogRecord(Info, LOG_GET_FUNC(), __LINE__, LOG_GET_FILE())
              << "Logger " << instance << " Initialized";
    return logger;
}

/// Initializes a logger with RollingFileWriter with any Formatter (specific
/// logger instance)
template <class Formatter, int instance>
inline Logger<instance>& init_logger(Severity max_severity,
    const char* filename,
    size_t max_file_size = 0,
    int max_files = 0) {
    static RollingFileWriter<Formatter> rollingFilewriter(
        filename, max_file_size, max_files);
    return init_logger<instance>(max_severity, &rollingFilewriter);
}

/// Initializes a logger with RollingFileWriter and TxtFormatter
template <int instance>
inline Logger<instance>& init_logger(Severity max_severity,
                         const char* filename,
                         size_t max_file_size = 0,
                         int max_files = 0) {
    return init_logger<TxtFormatter, instance>(max_severity, filename,
        max_file_size, max_files);
}

//==============================================================================
// DEFAULT MAHI LOGGER
//==============================================================================

/// Built in default Logger. Contains two writers: (0) a RollingFileWriter with a
/// TxtFormatter and default severity Verbose, and (1) a ColorConsoleWriter with
/// TxtFormatter and default severity Info. Can be disabled by undefining
/// MAHI_DEFAULT_LOG or enabling DISABLE_LOG option in CMakeLists.txt
extern Logger<DEFAULT_LOGGER>* MahiLogger;

} // namespace util
} // namespace mahi

//==============================================================================
// LOGGING MACRO FUNCTIONS
//==============================================================================

/// Log severity level checker for specific logger instance
#define IF_LOG_(instance, severity)                          \
    if (!mahi::util::get_logger<instance>() ||                           \
        !mahi::util::get_logger<instance>()->check_severity(severity)) { \
        ;                                                    \
    } else

/// Main logging macro for specific logger instance
#define LOG_(instance, severity) \
    IF_LOG_(instance, severity)  \
    (*mahi::util::get_logger<instance>()) += mahi::util::LogRecord(severity, LOG_GET_FUNC(), __LINE__, LOG_GET_FILE())

/// Conditional logging macro for specific logger instance
#define LOG_IF_(instance, severity, condition) \
    if (!(condition)) {                        \
        ;                                      \
    } else                                     \
        LOG_(instance, severity)

/// Log severity level checker for default MAHI logger
#define IF_LOG(severity)                                        \
    if (!mahi::util::MahiLogger || !mahi::util::MahiLogger->check_severity(severity)) { \
        ;                                                       \
    } else

/// Main logging macro for default MAHI logger
#define LOG(severity) \
    IF_LOG(severity)  \
        *mahi::util::MahiLogger += mahi::util::LogRecord(severity, LOG_GET_FUNC(), __LINE__, LOG_GET_FILE())

/// Conditional logging macro for default MAHI logger
#define LOG_IF(severity, condition) \
    if (!(condition)) {             \
        ;                           \
    } else                          \
        LOG(severity)

