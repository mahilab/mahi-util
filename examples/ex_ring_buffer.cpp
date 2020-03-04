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

// Usage:
// Interact with the RingBuffer class through the command line. Use the possible
// methods seen below, followed by a space, followed by the argument, e.g.
// Terminal: push_back 1
// Terminal: push_back 2
// Terminal: push_back 3
// Terminal: pop_back
// Terminal: [] 1

int main() {
    RingBuffer<int> x(5);
    std::string method;
    int idx;
    int value;
    while (true) {
        std::cin >> method;
        if (method == "push_back") {
            std::cin >> value;
            x.push_back(value);
        } else if (method == "push_front") {
            std::cin >> value;
            x.push_front(value);
        } else if (method == "pop_back") {
            println("a: ");
            println(x.pop_back());
        } else if (method == "pop_front") {
            println("a: ");
            println(x.pop_front());
        } else if (method == "[]") {
            std::cin >> idx;
            println("a: ");
            println(x[idx]);
        } else if (method == "[]=") {
            std::cin >> idx;
            std::cin >> value;
            println("a: ");
            x[idx] = value;
        } else if (method == "resize") {
            std::cin >> value;
            x.resize(value);
        } else if (method == "clear") {
            x.clear();
        }
        for (std::size_t i = 0; i < x.size(); ++i) {
            std::cout << x[i] << " ";
        }
        std::cout << std::endl << "\n";
    }
    return 0;
}
