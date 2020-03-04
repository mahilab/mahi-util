#include <Mahi/Util/Logging/Csv.hpp>
#include <thread>

namespace mahi {
namespace util {    

Csv::Csv() : File(), precision_(6) {}

Csv::Csv(const std::string &filepath, WriteMode w_mode, OpenMode o_mode) :
    File(filepath, w_mode, o_mode),
    precision_(6)
{

}

void Csv::set_precision(std::size_t precision) {
    precision_ = precision;
}

} // util
} // mahi
