#include <Mahi/Util/Logging/Log.hpp>

namespace mahi {
namespace util {

#ifdef MAHI_DEFAULT_LOG
    static ColorConsoleWriter<TxtFormatter> default_console_writer(Info);
    Logger<DEFAULT_LOGGER>* MahiLogger = &init_logger<DEFAULT_LOGGER>(Verbose, "MAHI.log", 256000, 10).add_writer(&default_console_writer);
#else
    Logger<DEFAULT_LOGGER>* MahiLogger = nullptr;
#endif

} // namespace util
} // namespace mahi