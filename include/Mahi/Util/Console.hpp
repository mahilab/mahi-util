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

/// Key Codes
enum {
    KEY_ESCAPE    = 0,
    KEY_ENTER     = 1,
    KEY_BACKSPACE = 8,
    KEY_SPACE     = 32,

    KEY_INSERT = 2,
    KEY_HOME   = 3,
    KEY_PGUP   = 4,
    KEY_DELETE = 5,
    KEY_END    = 6,
    KEY_PGDOWN = 7,

    KEY_UP    = 14,
    KEY_DOWN  = 15,
    KEY_LEFT  = 16,
    KEY_RIGHT = 17,

    KEY_F1  = 18,
    KEY_F2  = 19,
    KEY_F3  = 20,
    KEY_F4  = 21,
    KEY_F5  = 22,
    KEY_F6  = 23,
    KEY_F7  = 24,
    KEY_F8  = 25,
    KEY_F9  = 26,
    KEY_F10 = 27,
    KEY_F11 = 28,
    KEY_F12 = 29,

    KEY_NUMDEL  = 30,
    KEY_NUMPAD0 = 31,
    KEY_NUMPAD1 = 127,
    KEY_NUMPAD2 = 128,
    KEY_NUMPAD3 = 129,
    KEY_NUMPAD4 = 130,
    KEY_NUMPAD5 = 131,
    KEY_NUMPAD6 = 132,
    KEY_NUMPAD7 = 133,
    KEY_NUMPAD8 = 134,
    KEY_NUMPAD9 = 135
};

}  // namespace util
}  // namespace mahi