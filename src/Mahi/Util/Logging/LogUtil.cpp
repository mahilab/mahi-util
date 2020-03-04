#include <Mahi/Util/Logging/Detail/LogUtil.hpp>
#include <cstring>

namespace mahi {
namespace util {

    LogRecord::LogRecord(Severity severity,
        const char* func,
        size_t line,
        const char* file,
        Timestamp timestamp)
        : timestamp_(timestamp),
        severity_(severity),
        tid_(mahi::util::get_thread_id()),
        line_(line),
        func_(func),
        file_(file)
    {
    }

    LogRecord::~LogRecord() { }

    LogRecord& LogRecord::operator <<(char data) {
        char str[] = { data, 0 };
        return *this << str;
    }

    LogRecord& LogRecord::operator <<(std::ostream& (*data)(std::ostream&))
    {
        message_ << data;
        return *this;
    }

    const char* LogRecord::get_message() const {
        message_str_ = message_.str();
        return message_str_.c_str();
    }

    const mahi::util::Timestamp& LogRecord::get_timestamp() const { return timestamp_; };

    Severity LogRecord::get_severity() const { return severity_; }

    unsigned int LogRecord::get_tid_() const { return tid_; }

    size_t LogRecord::get_line() const { return line_; }

    const char* LogRecord::get_func() const {
        func_str_ = process_function_name(func_);
        return func_str_.c_str();
    }

    const char* LogRecord::get_file() const { return file_; }

    inline std::string process_function_name(const char* func) {
#if (defined(_WIN32) && !defined(__MINGW32__)) || defined(__OBJC__)
        return std::string(func);
#else
        const char* funcBegin = func;
        const char* funcEnd = ::strchr(funcBegin, '(');

        if (!funcEnd) {
            return std::string(func);
        }
        for (const char* i = funcEnd - 1; i >= funcBegin;
            --i)  // search backwards for the first space char
        {
            if (*i == ' ') {
                funcBegin = i + 1;
                break;
            }
        }
        return std::string(funcBegin, funcEnd);
#endif
    }


} // namespace util
} // namespace mahi
