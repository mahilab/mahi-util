#include <Mahi/Util/Math/Chirp.hpp>
#include <Mahi/Util/Math/Constants.hpp>
#include <cmath>

namespace mahi {
namespace util {

Chirp::Chirp(Frequency _start, Frequency _final, Time _T, double _amplitude, double _offset) :
    start(_start),
    final(_final),
    T(_T),
    amplitude(_amplitude),
    offset(_offset)
{
}

double Chirp::evaluate(Time t) {

    double ts = t.as_seconds();
    double Ts = T.as_seconds();

    if (ts > Ts)
        return 0.0;

    double f0 = static_cast<double>(start.as_hertz());
    double f1 = static_cast<double>(final.as_hertz());

    double k   = (f1 - f0) / Ts;
    double phi = 2 * PI * (f0 * ts + 0.5 * k * ts * ts);

    double value = std::sin(phi);
    value *= amplitude;
    value += offset;
    return value;
}

} // namespace util
} // namespace mahi
