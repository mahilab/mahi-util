#include <Mahi/Util/Coroutine.hpp>
#include <chrono>

namespace mahi {
namespace util {

//==============================================================================
// YieldInstruction
//==============================================================================

YieldInstruction::YieldInstruction() {
}

YieldInstruction::~YieldInstruction() {
}

bool YieldInstruction::is_over() {
   return true;
}

WaitForSeconds::WaitForSeconds(double sec) :
    WaitForSeconds(seconds(sec))
{ }

WaitForSeconds::WaitForSeconds(Time duration) :
   m_dur(duration)
{
    m_clk.restart();
}

bool WaitForSeconds::is_over() {
    return m_clk.get_elapsed_time() >= m_dur;
}

WaitUntil::WaitUntil(std::function<bool()> func) :
    m_func(func)
{
    
}

bool WaitUntil::is_over() {
    return m_func();
}

WaitWhile::WaitWhile(std::function<bool()> func) :
    m_func(func)
{
    
}

bool WaitWhile::is_over() {
    return !m_func();
}


//==============================================================================
// PromiseType
//==============================================================================

PromiseType::PromiseType() :
   m_instruction(nullptr)
{
}

PromiseType::~PromiseType() {
}

SuspendAlways PromiseType::initial_suspend() {
   return SuspendAlways{}; // changing this to change start
}

SuspendAlways PromiseType::final_suspend() {
   return SuspendAlways{};
}

Enumerator PromiseType::get_return_object() {
    auto h = std::experimental::coroutine_handle<PromiseType>::from_promise(*this);
    std::shared_ptr<Coroutine> coro(new Coroutine(h));
    return Enumerator(coro);
}

void PromiseType::unhandled_exception() {
   std::exit(1);
}

SuspendNever PromiseType::return_void() {
   return SuspendNever{};
}

SuspendAlways PromiseType::yield_value(YieldInstruction* value) {
   m_instruction = std::shared_ptr<YieldInstruction>(value);
   return SuspendAlways{};
}

SuspendAlways PromiseType::yield_value(std::shared_ptr<YieldInstruction> value) {
    m_instruction = value;
    return SuspendAlways{};
}

//==============================================================================
// Coroutine
//==============================================================================

Coroutine::Coroutine(std::experimental::coroutine_handle<PromiseType> coroutine) :
    YieldInstruction(),
    m_coroutine(coroutine),
    m_stop(false)
{
}

Coroutine::Coroutine(Coroutine &&other) :
    m_coroutine(other.m_coroutine)
{
    other.m_coroutine = nullptr;
}

Coroutine::~Coroutine() {
    if (m_coroutine) {
        m_coroutine.destroy();
    }
}

bool Coroutine::is_over() {
    return m_coroutine.done();
}

void Coroutine::stop() {
    m_stop = true;
}

//==============================================================================
// Enumerator
//==============================================================================

bool Enumerator::move_next() {
    if (m_ptr->m_stop) // coroutine has request stop
        return false;
    auto instruction = m_ptr->m_coroutine.promise().m_instruction;
    if (instruction) { // there is an instruction
        if (instruction->is_over()) { // yield instruction is over
            m_ptr->m_coroutine.resume();
            return !m_ptr->m_coroutine.done();
        }
        else { // yield instruction is not over
            return true;
        }
    }
    else { // no yield instruction
        m_ptr->m_coroutine.resume();
        return !m_ptr->m_coroutine.done();
    }
}

Enumerator::Enumerator(std::shared_ptr<Coroutine> h) :
    m_ptr(h)
{
}

Enumerator::~Enumerator() {

}

Enumerator::Enumerator(Enumerator &&e) :
    m_ptr(std::move(e.m_ptr))
{
    e.m_ptr = nullptr;
};

std::shared_ptr<Coroutine> Enumerator::get_coroutine() {
    return m_ptr;
}

} // namespace util
} // namespac mahi

