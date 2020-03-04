#include <Mahi/Util/Timing/Time.hpp>
#include <Mahi/Util/Timing/Frequency.hpp>
#include <limits>

namespace mahi {
namespace util {

//==============================================================================
// CLASS DEFINITIONS
//==============================================================================

const Time Time::Zero;
const Time Time::Inf = microseconds(std::numeric_limits<int64>::max()); // 29k years, effectively infinite :)

Time::Time() :
    microseconds_(0)
{
}

Time::Time(int64 microseconds) :
    microseconds_(microseconds)
{
}

double Time::as_seconds() const
{
    return microseconds_ / 1000000.0;
}

int32 Time::as_milliseconds() const
{
    return static_cast<int32>(microseconds_ / 1000);
}

int64 Time::as_microseconds() const
{
    return microseconds_;
}


Frequency Time::to_frequency() const {
    if (microseconds_ == std::numeric_limits<int64>::max())
        return Frequency::Zero;
    if (microseconds_ == 0)
        return Frequency::Inf;
    return megahertz(1.0 / static_cast<double>(microseconds_));
}

//==============================================================================
// INSTANTIATION FUNCTIONS
//==============================================================================

Time seconds(double amount) {
    return Time(static_cast<int64>(amount * 1000000));
}

Time milliseconds(int32 amount) {
    return Time(static_cast<int64>(amount) * 1000);
}

Time microseconds(int64 amount) {
    return Time(amount);
}

//==============================================================================
// USER DEFINED LITERALS
//==============================================================================

Time operator ""_s(long double ammount) {
    return seconds(ammount);
}

Time operator ""_s(unsigned long long int ammount) {
    return seconds((double)ammount);
}

Time operator ""_ms(unsigned long long int ammount) {
    return milliseconds((int32)ammount);
}

Time operator ""_us(unsigned long long int ammount) {
    return microseconds(ammount);
}

//==============================================================================
// OPERATOR OVERLOADS
//==============================================================================

std::ostream& operator << (std::ostream& os, const Time& t) {
    if (t.as_seconds() >= 1.0)
        os << t.as_seconds() << " s";
    else if (t.as_milliseconds() > 1)
        os << t.as_milliseconds() << " ms";
    else
        os << t.as_microseconds() << " us";
    return os;
}

bool operator ==(Time left, Time right) {
    return left.as_microseconds() == right.as_microseconds();
}

bool operator !=(Time left, Time right) {
    return left.as_microseconds() != right.as_microseconds();
}

bool operator <(Time left, Time right) {
    return left.as_microseconds() < right.as_microseconds();
}

bool operator >(Time left, Time right) {
    return left.as_microseconds() > right.as_microseconds();
}

bool operator <=(Time left, Time right) {
    return left.as_microseconds() <= right.as_microseconds();
}

bool operator >=(Time left, Time right) {
    return left.as_microseconds() >= right.as_microseconds();
}

Time operator -(Time right) {
    return microseconds(-right.as_microseconds());
}

Time operator +(Time left, Time right) {
    return microseconds(left.as_microseconds() + right.as_microseconds());
}

Time& operator +=(Time& left, Time right) {
    return left = left + right;
}

Time operator -(Time left, Time right) {
    return microseconds(left.as_microseconds() - right.as_microseconds());
}

Time& operator -=(Time& left, Time right) {
    return left = left - right;
}

Time operator *(Time left, double right) {
    return seconds(left.as_seconds() * right);
}

Time operator *(Time left, int64 right) {
    return microseconds(left.as_microseconds() * right);
}

Time operator *(double left, Time right) {
    return right * left;
}

Time operator *(int64 left, Time right) {
    return right * left;
}

Time& operator *=(Time& left, double right) {
    return left = left * right;
}

Time& operator *=(Time& left, int64 right) {
    return left = left * right;
}

Time operator /(Time left, double right) {
    return seconds(left.as_seconds() / right);
}

Time operator /(Time left, int64 right) {
    return microseconds(left.as_microseconds() / right);
}

Time& operator /=(Time& left, double right) {
    return left = left / right;
}

Time& operator /=(Time& left, int64 right) {
    return left = left / right;
}

double operator /(Time left, Time right) {
    return left.as_seconds() / right.as_seconds();
}

Time operator %(Time left, Time right) {
    return microseconds(left.as_microseconds() % right.as_microseconds());
}

Time& operator %=(Time& left, Time right) {
    return left = left % right;
}

} // namespace util
} // namespace mahi
