#include <Mahi/Util/Templates/SPSCQueue.hpp>
#include <Mahi/Util/System.hpp>
#include <Mahi/Util/Print.hpp>
#include <Mahi/Util/Timing/Clock.hpp>
#include <thread>
#include <string>
#include <iostream>

using namespace mahi::util;

// SPSCQueue is a single-producer-single-consumer, wait free, lock free queue for multithreadeding.
// It's a convenient way to send high throughput messages or data between threads without mutexing
// See: https://github.com/rigtorp/SPSCQueue for more documentation.

SPSCQueue<std::size_t> g_queue1(2048);

const std::size_t iters = 4096*4096*16;

void consumer() {
    std::size_t sum = 0;
    while (true) {
        while (!g_queue1.front());
        auto i = *g_queue1.front();
        sum += i;
        g_queue1.pop();
        if (i == iters-1)
            break;
    }
    print("Sum: {}",sum);
}

inline void producer() {
    for (std::size_t i = 0; i < iters; ++i) 
        g_queue1.push(i);
}

int main(int argc, char const *argv[])
{
    std::thread thrd(consumer);
    Clock clk;
    producer();
    thrd.join();
    print("Time: {} ms",clk.get_elapsed_time().as_milliseconds());
    return 0;
}
