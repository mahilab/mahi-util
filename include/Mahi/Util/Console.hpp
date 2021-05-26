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
#include <Mahi/Util/Timing/Time.hpp>
#include <Mahi/Util/Types.hpp>
#include <atomic>
#include <cxxopts.hpp>
#include <sstream>
#include <utility>

namespace mahi {
namespace util {

/// Import cxxopts library for command line Options class and utilites
using namespace cxxopts;

//==============================================================================
// SIGNAL HANDLING
//==============================================================================

/// Types of events that can be caught in in a console
enum class CtrlEvent {
    CtrlC    = 0,  ///< user pressed Ctrl+C
    CtrlQuit = 1,  ///< user pressed Ctrl+Break (Windows) or Ctrl+\ (Unix)
    Close    = 2,  ///< user has closed the console (Windows only)
    Logoff   = 3,  ///< user is logging off (Windows only)
    Shutdown = 4   ///< system is shutting down (Windows only)
};

/// Registers a function so that it is called when Ctrl+C is pressed.
///
/// The handler should have an unsigned long input parameter and return an int.
/// The input can be checked against the the control event values above for
/// further processsing. If this function handles the event, it should return
/// true, otherwise it should return false.
bool register_ctrl_handler(bool (*handler)(CtrlEvent));

/// Special bool type that can safely be used by a Ctrl handler
typedef volatile std::atomic<bool> ctrl_bool;

//==============================================================================
// CONSOLE COLORS
//==============================================================================

/// Represents a console text color
enum class ConsoleColor {
    None,
    Black,
    Gray,
    White,
    Red,
    DarkRed,
    Green,
    DarkGreen,
    Blue,
    DarkBlue,
    Cyan,
    Aqua,
    Magenta,
    Purple,
    Yellow,
    Gold
};

/// Sets the foreground and background text color in the console (thread-safe)
void set_text_color(ConsoleColor foreground, ConsoleColor background = ConsoleColor::None);

/// Resets the foreground and background text color to the default style (thread-safe)
void reset_text_color();

/// Enables virtual console (needed in Windows PowerShell/CMD if using fmt::color with fmt::print
/// aka mahi::util::print)
void enable_virtual_console();

//==============================================================================
// CONSOLE INPUT
//==============================================================================

/// Determines if keyboard has been hit (thread-safe)
int kb_hit();

/// Get character without waiting for Return to be pressed (blocking) (thread-safe)
int get_ch();

/// Get character without waiting for Return to be pressed (non-blocking) (thread-safe)
/// Returns 0 if no key was pressed.
int get_ch_nb();

/// Reads a key press and returns a key code (blocking) (thread-safe)
int get_key();

/// Reads a key press and returns a key code (blocking) (thread-safe)
/// Returns 0 if no key was pressed.
int get_key_nb();

//==============================================================================
// MISC
//==============================================================================

/// Returns true if stdout is a character device (thread-safe)
bool is_tty();

/// Causes the console to emit a beep sound (thread-safe)
void beep();

/// Clears screen, resets all attributes and moves cursor home (thread-safe)
void cls();

constexpr int AsciiEscape = 200; // arbitrary number to represent escape sequence before enums

/// Key Codes based on microsoft ascii codes https://docs.microsoft.com/en-us/previous-versions/visualstudio/visual-studio-6.0/aa299374(v=vs.60)
enum {
    KEY_BACKSPACE = 8,
    KEY_TAB       = 9,
    KEY_ENTER     = 13,
    KEY_ESCAPE    = 27,
    KEY_SPACE     = 32,

    KEY_NUM0 = 48,
    KEY_NUM1 = 49,
    KEY_NUM2 = 50,
    KEY_NUM3 = 51,
    KEY_NUM4 = 52,
    KEY_NUM5 = 53,
    KEY_NUM6 = 54,
    KEY_NUM7 = 55,
    KEY_NUM8 = 56,
    KEY_NUM9 = 57,

    KEY_F1  = AsciiEscape + 59,
    KEY_F2  = AsciiEscape + 60,
    KEY_F3  = AsciiEscape + 61,
    KEY_F4  = AsciiEscape + 62,
    KEY_F5  = AsciiEscape + 63,
    KEY_F6  = AsciiEscape + 64,
    KEY_F7  = AsciiEscape + 65,
    KEY_F8  = AsciiEscape + 66,
    KEY_F9  = AsciiEscape + 67,
    KEY_F10 = AsciiEscape + 68,
    KEY_F11 = AsciiEscape + 69,
    KEY_F12 = AsciiEscape + 70,

    KEY_HOME   = AsciiEscape + 71,
    KEY_UP     = AsciiEscape + 72,
    KEY_PGUP   = AsciiEscape + 73,
    KEY_LEFT   = AsciiEscape + 75,
    KEY_RIGHT  = AsciiEscape + 77,
    KEY_END    = AsciiEscape + 79,
    KEY_DOWN   = AsciiEscape + 80,
    KEY_PGDOWN = AsciiEscape + 81,
    KEY_INSERT = AsciiEscape + 82,
    KEY_DELETE = AsciiEscape + 83
};

}  // namespace util
}  // namespace mahi