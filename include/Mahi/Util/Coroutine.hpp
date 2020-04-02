#pragma once

#include <Mahi/Util/Timing/Clock.hpp>
#include <experimental/coroutine>
#include <memory>
#include <functional>


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
struct WaitForSeconds : public YieldInstruction {
    WaitForSeconds(double sec);
    WaitForSeconds(Time duration);
    bool is_over() override;
private:
    Clock m_clk;
    Time m_dur;
};

/// Yield instruction which waits until the supplied function evalutes to true
struct WaitUntil : public YieldInstruction {
    WaitUntil(std::function<bool()> func);
    bool is_over() override;
private:
    std::function<bool()> m_func;
};

/// Yield instruction which waits until the supplied function evalutes to true
struct WaitWhile : public YieldInstruction {
    WaitWhile(std::function<bool()> func);
    bool is_over() override;
private:
    std::function<bool()> m_func;
};

//==============================================================================
// PromiseType
//==============================================================================

struct PromiseType {
    PromiseType();
    ~PromiseType();
    SuspendAlways initial_suspend();
    SuspendAlways final_suspend();
    Enumerator get_return_object();
    void unhandled_exception();
    SuspendNever  return_void();
    SuspendAlways yield_value(YieldInstruction* value);
    SuspendAlways yield_value(std::shared_ptr<YieldInstruction>&& value);
    std::shared_ptr<YieldInstruction> m_instruction;
};

//==============================================================================
// Coroutine
//==============================================================================

struct Coroutine : public YieldInstruction
{
    /// Destructor
    ~Coroutine();
    /// Stops the Coroutine
    void stop();
    /// Returns true if the Coroutine is over
    bool is_over() override;
    /// Move semantics
    Coroutine(Coroutine &&other);

private:

    friend class Enumerator;
    friend struct PromiseType;

    /// Constructor
    Coroutine(std::experimental::coroutine_handle<PromiseType> coroutine);

private:
    std::experimental::coroutine_handle<PromiseType> m_coroutine; ///< underlying handle
    bool m_stop;
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
    Enumerator(Enumerator &&e);
    /// Advances Enumerator and returns true until completion
    bool move_next();
    /// Gets the Coroutine
    std::shared_ptr<Coroutine> get_coroutine();

private:
    friend struct PromiseType;
    /// Constructor
    Enumerator(std::shared_ptr<Coroutine> h);
private:
    std::shared_ptr<Coroutine> m_ptr;
};

} // namespace util
} // namespace mahi
