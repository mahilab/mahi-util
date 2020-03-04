#include <Mahi/Util/Concurrency/Spinlock.hpp>

namespace mahi {
namespace util {

void Spinlock::lock() {
    while(lock_.test_and_set(std::memory_order_acquire)) {
        ; // busy wait
    }
}

void Spinlock::unlock() {
    lock_.clear(std::memory_order_release);
}

} // namespace util
} // namespace mahi


