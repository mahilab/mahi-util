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
// Author(s): Craig McDonald (craig.g.mcdonald@gmail.com)

#include <Mahi/Util.hpp>

using namespace mahi::util;

// Usage:
// Run the example to generate a random signal that is passed through various
// filters and saved to a csv file.

int main() {

    // construct filters
    Butterworth lp_filter(2, 0.05, Butterworth::Lowpass);
    Butterworth hp_filter(2, 0.05, Butterworth::Highpass);
    Butterworth lps_filter(2, 0.05, Butterworth::Lowpass, 50);
    Butterworth hps_filter(2, 0.05, Butterworth::Highpass, 50);

    // number of samples to generate
    int samples = 250;

    // chaotic logistic map with offset as input signal
    double r = 3.78;
    double o = 5.0;
    double x = 0.5 + o;

    // initialize data logger to write immediately to file
    Csv csv("filter.csv");
    csv.write_row("Input", "LPF Output", "HPF Output", "LPF Seeded Output", "HPF Seeded Output");

    // data storage container
    std::vector<double> data(5);

    // begin filtering and logging data
    for (int i = 0; i < samples; ++i) {
        // generate input signal
        x = r * (x - o) * (1 - (x - o)) + o;

        // filter input signal and store data
        data[0] = x;
        data[1] = lp_filter.update(x);
        data[2] = hp_filter.update(x);
        data[3] = lps_filter.update(x);
        data[4] = hps_filter.update(x);

        // write to data log buffer
        csv.write_row(data);
    }

}
