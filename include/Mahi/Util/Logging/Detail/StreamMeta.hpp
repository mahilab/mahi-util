#pragma once
#include <sstream>

// Template meta-programming magic for stream operations in mahi::util

namespace mahi {
namespace util {
namespace detail {

inline void operator<<(std::ostringstream &stream, const char *data) {
    data = data ? data : "(null)";
    std::operator<<(stream, data);
}

inline void operator<<(std::ostringstream &stream, const std::string &data) {
    mahi::util::detail::operator<<(stream, data.c_str());
}

#ifdef _WIN32
namespace meta {

template <class T, class Stream>
inline char operator<<(Stream &, const T &);

template <class T, class Stream>
struct Streamable
{
    enum
    {
        value = sizeof(operator<<(*reinterpret_cast<Stream *>(0), *reinterpret_cast<const T *>(0))) != sizeof(char)
    };
};

template <class Stream>
struct Streamable<std::ios_base &(std::ios_base &), Stream>
{
    enum
    {
        value = true
    };
};

template <class Stream, size_t N>
struct Streamable<wchar_t[N], Stream>
{
    enum
    {
        value = false
    };
};

template <class Stream, size_t N>
struct Streamable<const wchar_t[N], Stream>
{
    enum
    {
        value = false
    };
};

template <bool B, class T = void>
struct enableIf
{
};

template <class T>
struct enableIf<true, T>
{
    typedef T type;
};
} // namespace meta

template <class T>
inline typename meta::enableIf<meta::Streamable<T, std::ostream>::value &&
                                   !meta::Streamable<T, std::wostream>::value,
                               void>::type
operator<<(std::wostringstream &stream, const T &data)
{
    std::ostringstream ss;
    ss << data;
    stream << ss.str();
}

#endif

} // namespace detail
} // namepace util
} // namespace mahi