<p align="center">
<img src="https://github.com/mahilab/mahi-util/blob/master/mahi-util.png" width="256"> 
</p>

## mahi::util

This library provides several helpful utility classes and functions that are consumed other `mahi-lib` libraries.

### Integration

The library is small and intended to be used with CMake's `FetchContent`:

```cmake
include(FetchContent) 
FetchContent_Declare(mahi-util GIT_REPOSITORY https://github.com/mahilab/mahi-util.git) 
FetchContent_MakeAvailable(mahi-util)

add_executable(my_app "my_app.cpp")
target_link_libraries(my_app mahi::util)
```

That's it! You should also be able to install or use the library as a git-submodule + CMake subdirectory if you prefer.
