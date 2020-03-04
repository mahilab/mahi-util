#include <Mahi/Util/Templates/SPSCQueue.hpp>
#include <Mahi/Util/System.hpp>
#include <thread>
#include <string>
#include <iostream>

using namespace mahi::util;

// SPSCQueue is a single-producer-single-consumer, wait free, lock free queue for multithreadeding.
// It's a convenient way to send high throughput messages or data between threads without mutexing
// See: https://github.com/rigtorp/SPSCQueue for more documentation.

SPSCQueue<std::string> g_queue(256);

void thrd_func() {
    while (true) {
        while (!g_queue.front());
        auto msg = *g_queue.front();
        g_queue.pop();
        std::cout << msg << std::endl;
        if (msg == "END")
            break;
    }
}

int main(int argc, char const *argv[])
{
    std::thread thrd(thrd_func);
    for (int i = 0; i < 512; ++i) 
        g_queue.push(std::to_string(i));
    g_queue.push("END");
    thrd.join();
    return 0;
}
