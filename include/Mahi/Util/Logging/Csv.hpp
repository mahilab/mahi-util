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

#include <Mahi/Util/Logging/File.hpp>
#include <Mahi/Util/NonCopyable.hpp>
#include <Mahi/Util/System.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <ios>

namespace mahi {
namespace util {

/// Represents an instance of a Comma-Separated Value (CSV) file
class Csv : public File {
public:

    /// Default constructor
    Csv();

    /// Constructor with filepath provided (opens file)
    Csv(const std::string& filepath, WriteMode w_mode = Truncate, OpenMode o_mode = OpenOrCreate);

    /// Writes a variable number of arguments to a new row, separated by commas
    template <typename Arg, typename... Args>
    void write_row(Arg&& arg, Args&&... args);

    /// Sets the precision of floating point values (default 6)
    void set_precision(std::size_t precision);
    
private:

    // hide functions inherited from File
    using File::unlink;
    using File::rename;
    using File::write;

private:

    std::size_t precision_;  ///< precision of floating point values
};

// The following free functions are provided for convenience and are not
// intended to be used in conjunction with a Csv instance. These functions
// can be used to quickly read/write/append homogenous data to a CSV file.
// Since they open and close the file on demand, you should not use these
// to continously access a file in a loop; prefer an instance of the Csv
// class instead. Containers can be any type which overloads operator[]
// for indexing, and provides a size() method for each dimensions
// (e.g vector, array, vector<vector>, vector<array>, RingBuffer<vector>, etc.)

/// Reads a single row into a 1D container. The container must be presized.
template <typename Container1D>
bool csv_read_row(const std::string &filepath, Container1D &data_out, std::size_t row_offset, std::size_t col_offset = 0);

/// Reads multiple rows into a 1D container. The container must be presized.
template <typename Container2D>
bool csv_read_rows(const std::string &filepath, Container2D &data_out, std::size_t row_offset = 0, std::size_t col_offset = 0);

/// Writes a 1D container to file
template <typename Container1D>
bool csv_write_row(const std::string &filepath, const Container1D &data_in);

/// Writes a 2D container to file
template <typename Container2D>
bool csv_write_rows(const std::string &filepath, const Container2D &data_in);

/// Appends a 1D container to file
template <typename Container1D>
bool csv_append_row(const std::string &filepath, const Container1D &data_in);

/// Appends a 2D container to file
template <typename Container2D>
bool csv_append_rows(const std::string &filepath, const Container2D &data_in);

} // namespace util
} // namespace mahi

#include <Mahi/Util/Logging/Detail/Csv.inl>