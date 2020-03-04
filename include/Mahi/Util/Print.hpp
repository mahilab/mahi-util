#pragma once
#include <string>
#include <fmt/format.h>

namespace mahi {
namespace util {

// Import fmt::print formatted printing
using fmt::print;

//==============================================================================
// VARIADIC PRITNING
//==============================================================================

/// Prints anything that works with stream operators and then starts a new line (thread-safe)
template <typename T>
void println(const T& value) {
    fmt::print("{}\n",value);
}

// Prints variadic number of arguments with separating spaces and then starts a new line (thread-safe)
template <typename Arg, typename... Args>
void println(Arg&& arg, Args&&... args) {
    std::stringstream ss;
    ss << std::forward<Arg>(arg);
    using expander = int[];
    (void)expander{0, (void(ss << ' ' << std::forward<Args>(args)), 0)...};
    ss << "\n";
    fmt::print("{}",ss.str());
}

/// Print with color (thread-safe)
template <typename T>
void color_println(const T& value, ConsoleColor foreground, ConsoleColor background = ConsoleColor::None) {
    set_text_color(foreground, background);
    print(value);
    reset_text_color();
}

} // namespace util
} // namesapce mahi