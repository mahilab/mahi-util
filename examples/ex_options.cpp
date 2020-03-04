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
//
#include <Mahi/Util.hpp>

using namespace mahi::util;

// The Options class in mahi::util is taken directly from the open-source cxxopts.
// Consult its documentation/wiki for more info: (github.com/jarro2783/cxxopts)

int main(int argc, char* argv[]) {
    // create Options object
    Options options("options.exe", "Simple Program Demonstrating Options");

    // add options
    // options can be short, long, or both (short must come first)
    // for example:   options.exe -e   OR   options.exe --evan
    options.add_options()
        ("e,evan", "Prints a message from Evan.")
        ("c,craig", "Prints a message from Craig.")
        ("h,help", "Prints helpful information.")
        ("i,integer", "Gets an integer from user.", value<int>())
        ("d,double", "Gets a double from user.", value<double>())
        ("v,vector", "Gets a vector of integers from user.", value<std::vector<int>>());

    // parse options
    auto result = options.parse(argc, argv);

    // do things with result:

    if (result.count("evan") > 0)
        println("Hello, my name is Evan!");

    if (result.count("c") > 0)
        println("Hello, my name is Craig!");

    if (result.count("help") > 0)
        println(options.help());

    if (result.count("integer") > 0) {
        int i = result["integer"].as<int>();
        println("You entered " + std::to_string(i));
    }

    if (result.count("d") > 0) {
        double d = result["d"].as<double>();
        println("You entered " + std::to_string(d));
    } 

    if (result.count("v")) {
        auto v = result["v"].as<std::vector<int>>();
        println(v.size());
        println(v);
    }

    return 0;
}
