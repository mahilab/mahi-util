#include <Mahi/Util/Concurrency/Mutex.hpp>
#include <Mahi/Util/NonCopyable.hpp>
#ifdef _WIN32
    #include <windows.h>
#else
    #include <pthread.h>
#endif

namespace mahi {
namespace util {

//==============================================================================
// PIMPL
//==============================================================================

class Mutex::Impl : NonCopyable
{
public:
    Impl();
    ~Impl();
    void lock();
    void unlock();
private:
#ifdef _WIN32
    CRITICAL_SECTION mutex_; ///< Win32 handle of the mutex
#else
    pthread_mutex_t mutex_; ///< pthread handle of the mutex
#endif
};

#ifdef _WIN32

//==============================================================================
// WINDOWS IMPLEMENTATION
//==============================================================================

Mutex::Impl::Impl() {
    InitializeCriticalSection(&mutex_);
}

Mutex::Impl::~Impl() {
    DeleteCriticalSection(&mutex_);
}

void Mutex::Impl::lock() {
    EnterCriticalSection(&mutex_);
}

void Mutex::Impl::unlock() {
    LeaveCriticalSection(&mutex_);
}

#else

//==============================================================================
// LINUX IMPLEMENTATION
//==============================================================================

Mutex::Impl::Impl() {
    // Make it recursive to follow the expected behavior
    pthread_mutexattr_t attributes;
    pthread_mutexattr_init(&attributes);
    pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);

    pthread_mutex_init(&mutex_, &attributes);
}

Mutex::Impl::~Impl() {
    pthread_mutex_destroy(&mutex_);
}

void Mutex::Impl::lock() {
    pthread_mutex_lock(&mutex_);
}

void Mutex::Impl::unlock() {
    pthread_mutex_unlock(&mutex_);
}

#endif

//==============================================================================
// CLASS DEFINITIONS
//==============================================================================

Mutex::Mutex() :
    impl_(new Mutex::Impl)
{
}

Mutex::~Mutex() {
}

void Mutex::lock() {
    impl_->lock();
}

void Mutex::unlock() {
    impl_->unlock();
}

} // namespace util
} // namespace mahi
