#pragma once

#include <Mahi/Util/Timing/Clock.hpp>
#include <experimental/coroutine>
#include <functional>
#include <memory>
#include <vector>

namespace mahi {
namespace util {

/// Coroutines modeled after Unity's courtines, using std::experimental::coroutine

//==============================================================================
// Forward Declarations / Typedefs
//==============================================================================

class Enumerator;
using SuspendAlways = std::experimental::suspend_always;
using SuspendNever  = std::experimental::suspend_never;

//==============================================================================
// YieldInsruction
//==============================================================================

/// Coroutine yield instruction base class
struct YieldInstruction {
    YieldInstruction();
    virtual ~YieldInstruction();
    virtual bool is_over();
};

/// Yield instruction which waits a certain duration in seconds
struct YieldTime : public YieldInstruction {
    YieldTime(Time duration);
    bool is_over() override;

private:
    Clock m_clk;
    Time  m_dur;
};

/// Makes a YieldTime instruction
inline std::shared_ptr<YieldTime> yield_time(Time duration) {
    return std::make_shared<YieldTime>(duration);
}

/// Yield instruction which waits until the supplied function evalutes to true
struct YieldUntil : public YieldInstruction {
    YieldUntil(std::function<bool()> func);
    bool is_over() override;

private:
    std::function<bool()> m_func;
};

/// Makes a YieldUntil instruction
inline std::shared_ptr<YieldUntil> yield_until(std::function<bool()> func) {
    return std::make_shared<YieldUntil>(func);
}

/// Yield instruction which waits until the supplied function evalutes to true
struct YieldWhile : public YieldInstruction {
    YieldWhile(std::function<bool()> func);
    bool is_over() override;

private:
    std::function<bool()> m_func;
};

/// Makes a YieldWhile instruction
inline std::shared_ptr<YieldWhile> yield_while(std::function<bool()> func) {
    return std::make_shared<YieldWhile>(func);
}

//==============================================================================
// PromiseType
//==============================================================================

struct PromiseType {
    PromiseType();
    ~PromiseType();
    SuspendAlways                     initial_suspend();
    SuspendAlways                     final_suspend();
    Enumerator                        get_return_object();
    void                              unhandled_exception();
    SuspendNever                      return_void();
    SuspendAlways                     yield_value(YieldInstruction* value);
    SuspendAlways                     yield_value(std::shared_ptr<YieldInstruction>&& value);
    std::shared_ptr<YieldInstruction> m_instruction;
};

//==============================================================================
// Coroutine
//==============================================================================

struct Coroutine : public YieldInstruction {
    /// Destructor
    ~Coroutine();
    /// Stops the Coroutine
    void stop();
    /// Returns true if the Coroutine is over
    bool is_over() override;
    /// Move semantics
    Coroutine(Coroutine&& other);

private:
    friend class Enumerator;
    friend struct PromiseType;

    /// Constructor
    Coroutine(std::experimental::coroutine_handle<PromiseType> coroutine);

private:
    std::experimental::coroutine_handle<PromiseType> m_coroutine;  ///< underlying handle
    bool                                             m_stop;
};

//==============================================================================
// Enumerator
//==============================================================================

class Enumerator {
public:
    using promise_type = PromiseType;

    /// Destructor
    ~Enumerator();
    /// Move semantics
    Enumerator(Enumerator&& e);
    /// Advances Enumerator and returns true until completion
    bool step();
    /// Gets the Coroutine
    std::shared_ptr<Coroutine> get_coroutine();

private:
    friend struct PromiseType;
    /// Constructor
    Enumerator(std::shared_ptr<Coroutine> h);

private:
    std::shared_ptr<Coroutine> m_ptr;
};

//==============================================================================
// Coroutine Manager
//==============================================================================

/// Utility class to help manage the the lifetime of Coroutines
class CoroutineManager {
public:
    /// Starts a coroutine
    std::shared_ptr<Coroutine> start(Enumerator &&coro);
    /// Stops an already running coroutine
    void stop(std::shared_ptr<Coroutine> coro);
    /// Stops all running coroutines
    void stop_all();
    /// Returns the number of coroutines running
    int count() const;
    /// Steps all running coroutines, usually called at a regular interval 
    /// (e.g. loop or refresh rate). If you are an end user (i.e.. you didn't
    /// construct this manager yourself), you likely don't want to call this!
    void step_all();
private:
    /// Vector of running coroutines
    std::vector<Enumerator> m_coroutines;  
};

}  // namespace util
}  // namespace mahi
