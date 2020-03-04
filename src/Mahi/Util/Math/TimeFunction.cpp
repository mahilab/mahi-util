#include <Mahi/Util/Math/TimeFunction.hpp>

namespace mahi {
namespace util {

    double TimeFunction::operator()(Time t) {
        return evaluate(t);
    }

} // namespace util
} // namespace mahi