target_sources(util
    PRIVATE
    Console.cpp
    Print.cpp
    Random.cpp
    System.cpp
    Types.cpp
)

if (MAHI_UTIL_COROUTINES)
    target_sources(util PRIVATE Coroutine.cpp)
endif()

add_subdirectory(Concurrency)
add_subdirectory(Logging)
add_subdirectory(Timing)
