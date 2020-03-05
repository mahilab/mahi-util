#pragma once

#include <Mahi/Util/Console.hpp>
#include <Mahi/Util/Device.hpp>
#include <Mahi/Util/Print.hpp>
#include <Mahi/Util/Event.hpp>
#include <Mahi/Util/NonCopyable.hpp>
#include <Mahi/Util/Random.hpp>
#include <Mahi/Util/StlStreams.hpp>
#include <Mahi/Util/System.hpp>
#include <Mahi/Util/Types.hpp>

#include <Mahi/Util/Concurrency/Lock.hpp>
#include <Mahi/Util/Concurrency/Mutex.hpp>
#include <Mahi/Util/Concurrency/NamedMutex.hpp>
#include <Mahi/Util/Concurrency/Spinlock.hpp>

#include <Mahi/Util/Math/Butterworth.hpp>
#include <Mahi/Util/Math/Chirp.hpp>
#include <Mahi/Util/Math/Constants.hpp>
#include <Mahi/Util/Math/Differentiator.hpp>
#include <Mahi/Util/Math/Filter.hpp>
#include <Mahi/Util/Math/Functions.hpp>
#include <Mahi/Util/Math/Integrator.hpp>
#include <Mahi/Util/Math/TimeFunction.hpp>
#include <Mahi/Util/Math/Waveform.hpp>

#include <Mahi/Util/Logging/Csv.hpp>
#include <Mahi/Util/Logging/Log.hpp>
#include <Mahi/Util/Logging/File.hpp>

#include <Mahi/Util/Templates/RingBuffer.hpp>
#include <Mahi/Util/Templates/SPSCQueue.hpp>
#include <Mahi/Util/Templates/Singleton.hpp>

#include <Mahi/Util/Timing/Clock.hpp>
#include <Mahi/Util/Timing/Frequency.hpp>
#include <Mahi/Util/Timing/Time.hpp>
#include <Mahi/Util/Timing/Timer.hpp>
#include <Mahi/Util/Timing/Timestamp.hpp>
#include <Mahi/Util/System.hpp>

#ifdef MAHI_COROUTINES
#include <Mahi/Util/Coroutine.hpp>
// #include <Mahi/Util/Range.hpp> C++17 only, TODO
#endif

// 3rd party includes
#include <fmt/format.h>
#include <fmt/printf.h>
#include <fmt/color.h>
#include <nlohmann/json.hpp>

namespace mahi {
namespace util {

// import all of json
using namespace nlohmann;

} // namespace util
} // namespace mahi