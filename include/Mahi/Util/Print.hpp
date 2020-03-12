#pragma once
#include <string>
#include <Mahi/Util/Console.hpp>
#include <Mahi/Util/StlStreams.hpp>
#include <fmt/format.h>
#include <fmt/color.h>

namespace mahi {
namespace util {

using namespace fmt::literals; // for _a

/// Same as fmt::print, but adds new line character
template <typename... Args>
void print(const char* format, const Args& ... args) {
    fmt::vprint(format, fmt::make_format_args(args...));
    fmt::print("\n");
}

/// Same as fmt::print, but adds new line character
template <typename... Args>
void print(const fmt::v6::text_style& ts, const char* format, const Args& ... args) {
    fmt::print(ts, format, args...);
    fmt::print("\n");
}

/// Prints anything that works with stream operators and then starts a new line
template <typename T>
void print_var(const T& value) {
    fmt::print("{}\n",value);
}

// Prints variadic number of arguments with separating spaces and then starts a new line
template <typename Arg, typename... Args>
void print_var(Arg&& arg, Args&&... args) {
    std::stringstream ss;
    ss << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(ss << ',' << std::forward<Args>(args)), 0)...};
    ss << "\n";
    fmt::print("{}",ss.str());
}

/// Prompts the user with a message and waits for Enter to be pressed (thread-safe)
template <typename... Args>
void prompt(const char* format, const Args& ... args) {
    fmt::vprint(format, fmt::make_format_args(args...));
    getchar();
}


} // namespace util
} // namesapce mahi