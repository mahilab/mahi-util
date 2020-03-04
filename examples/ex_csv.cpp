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

#include <Mahi/Util/Logging/Csv.hpp>
#include <Mahi/Util/Console.hpp>
#include <Mahi/Util/Random.hpp>
#include <Mahi/Util/Timing/Timer.hpp>
#include <Mahi/Util/Templates/RingBuffer.hpp>
#include <array>

using namespace mahi::util;
using namespace std;

int main() {

    //=========================================================================
    // Example 1: Basic writing with CSV free functions
    //=========================================================================

    // container for header strings (could also be array<string> )
    vector<string> header;
    // container for data (could also be vector<vector<T>>, vector<array<T>>, etc.)
    array<array<int ,10>, 10> data;
    // make some data
    for (std::size_t c = 0; c < 10; ++c) {
        header.push_back("Col_" + std::to_string(c));
        for (std::size_t r = 0; r < 10; ++r) {
            data[r][c] = random_range(0,100);
        }
    }

    // path to csv file (absolute_folder will be created in system root e.g. C:/)
    string filepath = "/absolute_folder/data1.csv";

    // write the header (just a row of strings, after all)
    csv_write_row(filepath, header);    
    // append the data
    csv_append_rows(filepath, data);

    //=========================================================================
    // Example 2: Basic reading with CSV free functions
    //=========================================================================

    // read back subset of header with offset
    array<string, 5> row;
    csv_read_row(filepath, row, 0, 2);
    print(row);
    // read back subset of data with offset as doubles
    array<array<double,5>,5> rows;
    csv_read_rows(filepath, rows, 1, 2);
    for (auto& r : rows) 
        print(r);

    //=========================================================================
    // Example 3: Logging a Rolling Window of Data
    //=========================================================================

    // write a RingBuffer of vectors (useful for logging a rolling window of data)
    RingBuffer<std::vector<double>> rolling_data(100); // only most recent 100 rows will be kept
    for (std::size_t i = 0; i < 150; ++i)
        rolling_data.push_back({(double)i,(double)i,(double)i});
    csv_write_rows("relative_folder/data2.csv", rolling_data); // relative_folder will be created in process directory

    //=========================================================================
    // Example 4: Advanced CSV usage with Csv instance
    //=========================================================================

    Csv csv("../sibling_folder/data3.csv");
    if (csv.is_open()) {
        // make a header
        csv.write_row("Time", "double", "string", "int", "vector[0]", "vector[1]", "vector[2]");
        // set precision of floating poitn numbers
        csv.set_precision(2);
        // simulate a loop
        print("Starting 10 second loop ...");
        Timer timer(hertz(1000));
        Time  t;
        while (t < seconds(10)) {
            // make some data
            double a_double         = random_range(0.0, 100.0);
            string a_string         = "string" + std::to_string(timer.get_elapsed_ticks());
            int a_int               = random_range(0, 1000);
            vector<double> a_vector = {random_range(0.0,1.0), random_range(1.0,2.0), random_range(2.0,3.0)};
            // write a row (note a_vector will count as 3 fields)
            csv.write_row(t, a_double, a_string, a_int, a_vector);
            t = timer.wait();
        }
        // print timer info to gauge HDD write performance
        print("Miss Rate: ", timer.get_miss_rate());
        print("Wait Ratio:", timer.get_wait_ratio());        
    }

    return 0;
}
