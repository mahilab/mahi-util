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

#include <Mahi/Util.hpp>

using namespace mahi::util;

ctrl_bool g_stop_flag(false);

bool my_handler(CtrlEvent event) {
    if (event == CtrlEvent::CtrlC)
        print("Ctrl-C Pressed");
    else if (event == CtrlEvent::CtrlQuit) {
        print("Ctrl-Break or Ctrl-\\ Pressed");
        g_stop_flag = true;
    }
    // ... check other Ctrl values as needed
    return 1;
}

int main() {
    print("Press Ctrl+C to continue, or Ctrl+Break to exit");
    register_ctrl_handler(my_handler);
    while (!g_stop_flag) {
    }
    return 0;
}
