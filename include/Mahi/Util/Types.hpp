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

namespace mahi {
namespace util {

typedef               char  int8;   ///<                       -127 to +127
typedef unsigned      char uint8;   ///<                          0 to +255
typedef              short  int16;  ///<                    -32,767 to +32,767
typedef unsigned     short uint16;  ///<                          0 to +65,535
typedef                int  int32;  ///<             -2,147,483,648 to +2,147,483,647
typedef unsigned       int uint32;  ///<                          0 to +4,294,967,295
#if defined(_MSC_VER)
typedef signed     __int64  int64;  ///< -9,223,372,036,854,775,808 to +9,223,372,036,854,775,807
typedef unsigned   __int64 uint64;  ///<                          0 to 18,446,744,073,709,551,615
#else
typedef signed   long long int64;   ///< -9,223,372,036,854,775,808 to +9,223,372,036,854,775,807
typedef unsigned long long uint64;  ///<                          0 to 18,446,744,073,709,551,615
#endif

} // namespace util
} // namespace mahi

