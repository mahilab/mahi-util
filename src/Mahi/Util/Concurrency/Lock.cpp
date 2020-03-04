#include <Mahi/Util/Concurrency/Lock.hpp>

namespace mahi {
namespace util {
//==============================================================================
// CLASS DEFINITIONS
//==============================================================================

Lock::Lock(Lockable& lockable) :
    lockable_(lockable)
{
    lockable_.lock();
}

Lock::~Lock() {
    lockable_.unlock();
}

} // namespace util
} // namespace mahi
