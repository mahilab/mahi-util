#include <Mahi/Util/Timing/Timestamp.hpp>
#include <sstream>
#include <iomanip>

#ifdef _WIN32
#include <sys/timeb.h>
#include <time.h>
#else
#include <sys/time.h>
#endif

namespace mahi {
namespace util {

inline void localtime_s(struct tm* t, const time_t* time) {
#if defined(_WIN32) && defined(__BORLANDC__)
    ::localtime_s(time, t);
#elif defined(_WIN32) && defined(__MINGW32__) && \
    !defined(__MINGW64_VERSION_MAJOR)
    *t = *::localtime(time);
#elif defined(_WIN32)
    ::localtime_s(t, time);
#else
    ::localtime_r(time, t);
#endif
}

#ifdef _WIN32
Timestamp::Timestamp() {
    timeb tb;
    ftime(&tb);
    tm t;
    mahi::util::localtime_s(&t, &tb.time);
    year     = t.tm_year + 1900;
    month    = t.tm_mon + 1;
    yday     = t.tm_yday + 1;
    mday     = t.tm_mday;
    wday     = t.tm_wday + 1;
    hour     = t.tm_hour;
    min      = t.tm_min;
    sec      = t.tm_sec;
    millisec = tb.millitm;
}
#else
Timestamp::Timestamp() {
    timeval tv;
    ::gettimeofday(&tv, NULL);
    tm t;
    mahi::util::localtime_s(&t, &tv.tv_sec);
    year     = t.tm_year + 1900;
    month    = t.tm_mon + 1;
    yday     = t.tm_yday + 1;
    mday     = t.tm_mday;
    wday     = t.tm_wday + 1;
    hour     = t.tm_hour;
    min      = t.tm_min;
    sec      = t.tm_sec;
    millisec = static_cast<unsigned short>(tv.tv_usec / 1000);
}
#endif

std::string Timestamp::yyyy_mm_dd() const {
    std::ostringstream ss;
    ss << year << "-"
        << std::setfill('0') << std::setw(2) << month << ("-")
        << std::setfill('0') << std::setw(2) << mday;
    return ss.str();
}

std::string Timestamp::hh_mm_ss_mmm() const {
    std::ostringstream ss;
    ss  << std::setfill('0') << std::setw(2) << hour << (":")
        << std::setfill('0') << std::setw(2) << min << (":")
        << std::setfill('0') << std::setw(2) << sec << (".")
        << std::setfill('0') << std::setw(3) << millisec;
    return ss.str();
}

std::string Timestamp::yyyy_mm_dd_hh_mm_ss() const {
    std::ostringstream ss;
    ss << year << "-"
        << std::setfill('0') << std::setw(2) << month << ("-")
        << std::setfill('0') << std::setw(2) << mday << ("_")
        << std::setfill('0') << std::setw(2) << hour << (".")
        << std::setfill('0') << std::setw(2) << min << (".")
        << std::setfill('0') << std::setw(2) << sec;
    return ss.str();
}

std::string Timestamp::yyyy_mm_dd_hh_mm_ss_mmm() const {
    std::ostringstream ss;
    ss << year << "-"
    << std::setfill('0') << std::setw(2) << month << ("-")
    << std::setfill('0') << std::setw(2) << mday << (" ")
    << std::setfill('0') << std::setw(2) << hour << (":")
    << std::setfill('0') << std::setw(2) << min << (":")
    << std::setfill('0') << std::setw(2) << sec << (".")
    << std::setfill('0') << std::setw(3) << millisec;
    return ss.str();
}

} // namespace util
} // namespace mahi

