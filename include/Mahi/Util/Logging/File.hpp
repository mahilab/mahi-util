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

#include <Mahi/Util/Types.hpp>
#include <Mahi/Util//NonCopyable.hpp>
#include <sstream>

namespace mahi {
namespace util {

/// Representats a file resource
class File : NonCopyable {
public:

    /// Default constructor
    File();

    /// Constructor with filepath provided (opens file)
    File(const std::string& filepath, WriteMode w_mode = Truncate, OpenMode o_mode = OpenOrCreate);

    /// Default destructor (closes file if open)
    virtual ~File();

    /// Opens the file for input-output operations
    bool open(const std::string &filepath, WriteMode mode = Truncate, OpenMode o_mode = OpenOrCreate);

    /// Writes to the file if the file is open
    int write(const void* data, std::size_t count);

    /// Writes to the file if the file is open
    template <class CharType>
    int write(const std::basic_string<CharType>& str) {
        return write(str.data(), str.size() * sizeof(CharType));
    }
    
    /// Returns true if file is open
    bool is_open();

    /// Closes the file if the file is open
    void close();

public:

    /// Removes a file if it exists
    static int unlink(const std::string& filepath);

    /// Renames a file if it exists
    static int rename(const std::string& old_filepath, const std::string& new_filepath);

protected:

    /// Repositions file offset from beginning
    off_t seek_set(off_t offset);

    /// Repositions file offset from current
    off_t seek_cur(off_t offset);

    /// Repositions file offset from end
    off_t seek_end(off_t offset);

    /// Repositions file offset according to directive whence and returns resulting offset
    off_t seek(off_t offset, int whence);

private:

    int file_handle_;  ///< file handle
};

} // namespace util
} // namespace mahi
