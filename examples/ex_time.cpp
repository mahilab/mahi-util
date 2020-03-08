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
#include <iostream>

using namespace mahi::util;

int main(int argc, char const *argv[])
{

    // Time
    Time t1 = 10_s; // alternatively, use seconds(10)
    Time t2 = 10_ms; // alternatively, use milliseconds(10)
    Time t3 = t1 + t2;
    if (t1 > t2)    
        fmt::print("{}\n",t3);

    double t3_s  = t3.as_seconds();
    int32  t3_ms = t3.as_milliseconds();
    int64  t3_us = t3.as_microseconds();
    print("{} s = {} ms = {} us", t3_s, t3_ms, t3_us);    

    // Frequency
    Frequency f1 = 1000_Hz;
    Frequency f2 = t2.to_frequency();
    print("{}, {}",f1,f2); 

    // Clock
    Clock clock;
    sleep(1_s);
    Time time1 = clock.get_elapsed_time();
    sleep(1_s);
    Time time2 = clock.restart();
    sleep(1_s);
    Time time3 = clock.get_elapsed_time();
    print("{}, {}, {}",time1, time2, time3);

    // Timer
    Timer timer1(1_s);
    time1 = timer1.wait();
    time2 = timer1.wait();
    time3 = timer1.wait();
    print("{}, {}, {}",time1, time2, time3);

    Timer hybrid_timer(milliseconds(10), Timer::WaitMode::Hybrid);
    print("{}",hybrid_timer.wait());

    // Time loops
    Time t = Time::Zero;
    Timer timer(1000_Hz);
    while (t < seconds(5)) {
        // code which executes in less than one millisecond
        t = timer.wait();
    }  

    return 0;
}
