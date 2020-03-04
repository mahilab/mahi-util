<p align="center">
<img src="https://github.com/mahilab/mahi-util/blob/master/mahi-util.png" width="256"> 
</p>

## mahi::util

This library provides several helpful utility classes and functions that are consumed other `mahi-lib` libraries.

### Integration

The library is small and intended to be copied to your build tree, or better yet, added as a `git` [submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules).

Suppose you already have a CMake project under `git` source control:

```shell
> cd my_project
my_project> git submodule add https://github.com/mahilab/mahi-util
```

The `mahi-util` directory will be cloned to the top level of `my_project`. Now, just add the following to your `CMakeLists.txt`:

```cmake
set(MAHI_UTIL_EXAMPLES OFF CACHE BOOL "" FORCE) # optional, ON by default
add_subdirectory(mahi-util)
add_executable(my_app "my_app.cpp")
target_link_libraries(my_app mahi::util)
```

That's it!
