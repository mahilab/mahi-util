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

int main() {

    // creat loop timer
    Timer timer(hertz(100), Timer::Hybrid);

    // create Differentiator and Integrators
    Differentiator differentiator;
    Integrator integrator = Integrator(5, Integrator::Technique::Simpsons);

    // create data logger
    Csv csv("math.csv");
    csv.write_row("t", "x(t)", "dx/dt", "dx/dt", "integral(x)", "integral(x)");
    std::vector<double> data(6);

    // loop
    while (timer.get_elapsed_time() < seconds(5)) {
        // get current time in seconds
        double t = timer.get_elapsed_time().as_seconds();
        data[0] = t;
        // x = 1/2 * [ cos(t) + 7 * cos(7t) ]
        data[1] = 0.5 * (std::cos(t) + 7.0 * std::cos(7.0 * t));
        // dx/dt = 1/2 * [-sin(t) - 49 * sin(7t)]
        data[2] = 0.5 * (-std::sin(t) - 49.0 * std::sin(7.0 * t));
        // dx/dt ~ differentiate(x);
        data[3] = differentiator.update(data[1], timer.get_elapsed_time());
        // integral(x) = sin(4t) * cos(3t) + 5
        data[4] = std::sin(4 * t) * std::cos(3 * t) + 5;
        // integral(x) ~ integrate(dx/dt)
        data[5] = integrator.update(data[1], timer.get_elapsed_time());
        // write data to csv
        csv.write_row(data);
        // wait timer
        timer.wait();
    }

    return 0;
}
