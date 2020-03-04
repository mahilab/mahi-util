#include <Mahi/Util/Math/Constants.hpp>
#include <limits>
#include <cmath>

namespace mahi {
namespace util {

//==============================================================================
// GENERIC
//==============================================================================

const double PI    = 3.141592653589793238462643383279502884;
const double HALFPI = PI * 0.5;
const double TWOPI = 2 * PI;
const double E     = 2.718281828459045235360287471352662497;
const double SQRT2 = std::sqrt(2.0);
const double SQRT3 = std::sqrt(3.0);
const double EPS   = std::numeric_limits<double>::epsilon();
const double INF   = std::numeric_limits<double>::infinity();
const double NaN   = std::numeric_limits<double>::quiet_NaN();

//==============================================================================
// CONVERSIONS
//==============================================================================

const double DEG2RAD    = PI / 180;
const double RAD2DEG    = 180 / PI;
const double INCH2METER = 0.0254;
const double METER2INCH = 1.0 / INCH2METER;

} // namespaec util
} // namespace mahi
