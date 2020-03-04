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

#include <Mahi/Util/Logging/Log.hpp>

using namespace mahi::util;

// Usage:
// Run the example to see messages logged to the command line with various
// levels of severity / formatting.

// custom loggers must start at 1 (the default logger is 0)
enum { MyLogger = 1 };

// custom formatters must define two public static functions:
// static std::string header() & static std::string format(const Record& record)
class MyFormatter {
public:
    static std::string header() { return "This is my header."; }

    static std::string format(const LogRecord& record) {
        std::string formatted_message = "";
        formatted_message += "Message: ";
        formatted_message += record.get_message();
        formatted_message += " (";
        formatted_message += record.get_timestamp().yyyy_mm_dd_hh_mm_ss();
        formatted_message += ")\n";
        return formatted_message;
    }
};

// custom writers must implement the Writer class and its single function write
template <class Formatter>
class MyWriter : public Writer {
public:
    virtual void write(const LogRecord& record) override {
        std::string str = Formatter::format(record);
        if (record.get_severity() == Fatal)
            set_text_color(Color::White, Color::Red);
        else if (record.get_severity() == Error)
            set_text_color(Color::Red, Color::DarkRed);
        else if (record.get_severity() == Warning)
            set_text_color(Color::Red, Color::Yellow);
        else if (record.get_severity() == Info)
            set_text_color(Color::Green, Color::DarkGreen);
        else
            set_text_color(Color::Cyan, Color::Blue);
        print_string(str);
        reset_text_color();
    }
};

int main() {

    /// Built in logger demonstration (only available if MAHI_DEFAULT_LOG was defined at compile time)
    LOG(Debug) << "This is a Debug log";      // goes nowhere by default
    LOG(Verbose) << "This is a Verbose log";  // goes to MEL.log only
    LOG(Info) << "This is an Info log";       // goes to MEL.log and console
    LOG(Warning) << "This is a Warning log";  // goes to MEL.log and console
    LOG(Error) << "This is an Error log";     // goes to MEL.log and console
    LOG(Fatal) << "This is a Fatal log";      // goes to MEL.log and console

    /// You can change the max severiy of the default logger
    MAHI_LOG->set_max_severity(Debug);

    LOG(Debug) << "This is another Debug log";      // goes to MEL.log and console
    LOG(Verbose) << "This is another Verbose log";  // goes to MEL.log and console

    //==========================================================================

    // You can create your own loggers too. First you need to choose a
    // Formatter. A Formatter is responsible for taking a log record and turning
    // it into a string. There are several built-in Formatters, and you can even
    // create your own. Here, we will use the built-in TxtFormatter. Once you've
    // chosen a Formatter, you create a Writer. A Writer is responsible for
    // taking a formatted string from a Formatter and writing it in some
    // prescribed manner, either to the console, a file, memory, etc. Here we
    // will choose the built int RollingFileWriter.
    RollingFileWriter<TxtFormatter> file_writer("my_log.txt");
    // Now, we create the actual Logger. Loggers are paramertized with an
    // integer which can be an enum for convienence. The default logger is 0, so
    // custom loggers should start at 1 or some other number.
    init_logger<MyLogger>(Verbose, &file_writer);
    /// Now we can do some logging. Note the slightly differnt macro name LOG_
    LOG_(MyLogger, Info) << "This is an Info log to MyLogger";
    LOG_(MyLogger, Error) << "This is an Error log to MyLogger";

    /// It's possible to add multiple writers to a single logger. Here, we will
    /// add our own custom Writer and Formatter to our existing logger.
    MyWriter<MyFormatter> my_writer;
    get_logger<MyLogger>()->add_writer(&my_writer);
    // note we could have done this at the beginning like so:
    // init_logger<MyLogger>(Verbose, &file_writer).>add_writer(&my_writer);

    LOG_(MyLogger, Debug) << "This is a custom Debug log";
    LOG_(MyLogger, Verbose) << "This is a custom Verbose log";
    LOG_(MyLogger, Info) << "This is an custom Info log";
    LOG_(MyLogger, Warning) << "This is a custom Warning log";
    LOG_(MyLogger, Error) << "This is an custom Error log";
    LOG_(MyLogger, Fatal) << "This is a custom Fatal log";

    return 0;
}
