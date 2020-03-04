#include <Mahi/Util/Math/Waveform.hpp>
#include <Mahi/Util/Math/Functions.hpp>
#include <Mahi/Util/Math/Constants.hpp>

namespace mahi {
namespace util {

Waveform::Waveform(Type _type, Time _period, double _amplitude, double _offset) :
    type(_type),
    period(_period),
    amplitude(_amplitude),
    offset(_offset)
{
}

Waveform::Waveform(Type _type, Frequency _frequency, double _amplitude, double _offset) :
    type(_type),
    period(_frequency.to_time()),
    amplitude(_amplitude),
    offset(_offset)
{

}

double Waveform::evaluate(Time t) {
    double seconds = t.as_seconds();
    double frequency = 1.0 / period.as_seconds();
    double value = 0.0;
    switch(type) {
        case Sin:
            value = sin(2.0 * PI * frequency * seconds);
            break;
        case Cos:
            value = cos(2.0 * PI * frequency * seconds);
            break;
        case Square:
            value = sin(2.0 * PI * frequency * seconds);
            if (value == 0.0)
                value = 0.0;
            else if (value > 0.0)
                value = 1.0;
            else
                value = -1.0;
            break;
        case Triangle:
            value = 2 / PI * asin(sin(2.0 * PI * frequency * seconds));
            break;
        case Sawtooth:
            value = -2 / PI * atan(cos(PI * frequency * seconds) / sin(PI * frequency * seconds));
            break;
    }
    value *= amplitude;
    value += offset;
    return value;
}

} // namespace util
} // namespace mahi


