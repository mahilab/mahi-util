#include <Mahi/Util/Timing/Frequency.hpp>
#include <Mahi/Util/Timing/Time.hpp>
#include <limits>

namespace mahi {
namespace util {

const Frequency Frequency::Zero;
const Frequency Frequency::Inf = hertz(std::numeric_limits<int64>::max());

Frequency::Frequency() :
    hertz_(0)
{

}

Frequency::Frequency(int64 hertz) :
    hertz_(hertz)
{

}

int64 Frequency::as_hertz() const {
    return hertz_;
}

int32 Frequency::as_kilohertz() const {
    return static_cast<int32>(hertz_ / 1000);
}

double Frequency::as_megahertz() const {
    return hertz_ / 1000000.0;
}

Time Frequency::to_time() const {
    if (hertz_ == std::numeric_limits<int64>::max())
        return Time::Zero;
    if (hertz_ == 0)
        return Time::Inf;
    return seconds(1.0 / static_cast<double>(hertz_));
}

//==============================================================================
// INSTANTIATION FUNCTIONS
//==============================================================================

Frequency hertz(int64 amount) {
    return Frequency(amount);
}

Frequency kilohertz(int32 amount) {
    return Frequency(static_cast<int64>(amount * 1000.0));
}

Frequency megahertz(double amount) {
    return Frequency(static_cast<int64>(amount * 1000000.0));
}

//==============================================================================
// USER DEFINED LITERALS
//==============================================================================

Frequency operator ""_Hz(unsigned long long int ammount) {
    return hertz(ammount);
}

Frequency operator ""_kHz(unsigned long long int ammount) {
    return kilohertz((int32)ammount);
}

Frequency operator ""_MHz(unsigned long long int ammount) {
    return megahertz((double)ammount);
}

Frequency operator ""_MHz(long double ammount) {
    return megahertz((double)ammount);
}

//==============================================================================
// OPERATOR OVERLOADS
//==============================================================================

bool operator==(Frequency left, Frequency right) {
    return left.as_hertz() == right.as_hertz();
}

std::ostream& operator << (std::ostream& os, const Frequency& t) {
    os << t.as_hertz() << " Hz";
    return os;
}

} // namespace util
} // namespace mahi
