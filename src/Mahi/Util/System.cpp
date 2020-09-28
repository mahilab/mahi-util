#include <Mahi/Util/System.hpp>
#include <Mahi/Util/Logging/Log.hpp>
#include <cstring>
#include <ctime>
#include <iomanip>

#ifdef _WIN32
#include <pdh.h>
#include <psapi.h>
#include <tchar.h>
#include <windows.h>
#else
#include <errno.h>
#include <pthread.h>
#include <sched.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sysctl.h>
#endif

namespace mahi {
namespace util {

//==============================================================================
// DIRECTORY FUNCTIONS
//==============================================================================

std::string get_separator() {
    #ifdef _WIN32
        return "\\";
    #else
        return "/";
    #endif
}

std::vector<std::string> split_path(std::string path)
{
    std::vector<std::string> directories;
    std::size_t found = path.find_first_of("/\\");
    while (found != std::string::npos) {
        std::string new_dir = path.substr(0, found);
        directories.push_back(new_dir);
        path = path.substr(found + 1, path.length());
        found = path.find_first_of("/\\");
    }
    directories.push_back(path);
    return directories;
}

bool is_root_dir(const std::string& dir) {
    if (dir == "")
        return true;
    else if (dir.length() == 2 && dir[1] == ':')
        return true;
    return false;
}

bool create_directory(const std::string &path)
{
    if (path == "" || path.empty())
        return true;
    std::vector<std::string> dirs = split_path(path);
    for (std::size_t i = 0; i < dirs.size(); ++i) {
        if (is_root_dir(dirs[i]))
            continue;
        std::string sub_path;
        for (std::size_t j = 0; j <= i; ++j) {
            sub_path += dirs[j];
            sub_path += get_separator();
        }
#ifdef _WIN32
        auto result = CreateDirectory(sub_path.c_str(), NULL);
        if (result == 0) {
            auto error = GetLastError();
            if (error == ERROR_ALREADY_EXISTS) {
                LOG(Verbose) << "Attempted to create directory " << sub_path << " which already exists";
            }
            else {
                LOG(Error) << "Failed to create directory " << sub_path << " (Windows Error #" << (int)error << ": " << get_last_os_error() << ")";
                return false;
            }
        }
#else
        int result = mkdir(sub_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if (result != 0) {
            LOG(Error) << "Failed to create directory " << sub_path << ". Ensure you have the correct permissions.";
            return false;
        }
#endif
    }
    return true;
}

bool directory_exits(std::string path) {
    if (path == "" || path.empty())
        return true;
    path = tidy_path(path, false);
    #ifdef _WIN32
        DWORD ftyp = GetFileAttributesA(path.c_str());
        if (ftyp == INVALID_FILE_ATTRIBUTES)
            return false; 
        if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
            return true; 
        return false; 
    #else
        struct stat sb;
        if (stat(path.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
            return true;
        return false;
    #endif
}

void split_filename(const std::string &filename_ext, std::string &filename, std::string &ext)
{
    const char *dot = std::strrchr(filename_ext.c_str(), '.');

    if (dot) {
        filename.assign(filename_ext.c_str(), dot);
        ext.assign(dot + 1);
    }
    else {
        filename.assign(filename_ext);
        ext.clear();
    }
}

std::string tidy_path(const std::string &in, bool is_file)
{
    if (in == "" || in.empty())
        return in;
    std::string out;
    auto dirs = split_path(in);
    for (std::size_t i = 0; i < dirs.size(); ++i)
    {
        if (dirs[i] == "")
        {
            if (i == 0)
                out += get_separator();
        }
        else if (dirs[i] != ".") {
            out += dirs[i] + get_separator();
        }
    }
    if (is_file)
        out.pop_back();
    return out;
}

bool parse_filepath(const std::string &in, std::string &directory, std::string &filename, std::string &ext, std::string &full)
{
    // can't do anythign with empty string
    if (in == "" || in.empty())
        return false;
    // split path
    auto splits = split_path(in);
    // split filename
    auto filename_ext = splits.back();
    if (filename_ext == "" || filename_ext.empty())
        return false;
    split_filename(filename_ext, filename, ext);
    // make directory string
    directory.clear();
    for (std::size_t i = 0; i < splits.size() - 1; ++i)
        directory += splits[i] + get_separator();
    directory = tidy_path(directory, false);
    full = directory + filename;
    if (ext != "" || !ext.empty())
        full += "." + ext;
    return true;
}

//==============================================================================
// SYSTEM FUNCTIONS
//==============================================================================

void sleep(Time duration) {
    if (duration >= Time::Zero) {
        #ifdef _WIN32
            TIMECAPS tc;
            timeGetDevCaps(&tc, sizeof(TIMECAPS));
            timeBeginPeriod(tc.wPeriodMin);
            // ::Sleep(duration.as_milliseconds()); // low-resolution method
            HANDLE timer;
            LARGE_INTEGER ft;
            ft.QuadPart = -(10 * duration.as_microseconds());
            timer = CreateWaitableTimer(NULL, TRUE, NULL);
            SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
            WaitForSingleObject(timer, INFINITE);
            CloseHandle(timer);
            // timeEndPeriod(tc.wPeriodMin); // to much overhead, not necessary?
        #else
            uint64 usecs = duration.as_microseconds();
            // Construct the time to wait
            timespec ti;
            ti.tv_nsec = (usecs % 1000000) * 1000;
            ti.tv_sec = usecs / 1000000;
            // If nanosleep returns -1, we check errno. If it is EINTR
            // nanosleep was interrupted and has set ti to the remaining
            // duration. We continue sleeping until the complete duration
            // has passed. We stop sleeping if it was due to an error.
            while ((nanosleep(&ti, &ti) == -1) && (errno == EINTR)) {
            }
        #endif
    }
}

std::string get_last_os_error() {
    #ifdef _WIN32
        //Get the error message, if any.
        DWORD errorMessageID = ::GetLastError();
        if (errorMessageID == 0)
            return std::string(); //No error message has been recorded
        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
        std::string message(messageBuffer, size);
        //Free the buffer.
        LocalFree(messageBuffer);
        return message;
    #else
        return std::string("get_last_os_error() not yet implemented on UNIX!");
    #endif
}

bool enable_realtime() {
    #ifdef _WIN32
        DWORD dwError;
        if (!SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS)) {
            dwError = GetLastError();
            LOG(Error) << "Failed to elevate process priority. Code: " << static_cast<int>(dwError);
            return false;
        }
        // DWORD dwPriClass;
        if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL)){
            dwError = GetLastError();
            LOG(Error) << "ERROR: Failed to elevate thread priority. Code: " << static_cast<int>(dwError);
            return false;
        }
        return true;
    #else
        int ret;
        // We'll operate on the currently running thread.
        pthread_t this_thread = pthread_self();
        // struct sched_param is used to store the scheduling priority
        struct sched_param params;
        // We'll set the priority to the maximum.
        params.sched_priority = sched_get_priority_max(SCHED_FIFO);
         // Attempt to set thread real-time priority to the SCHED_FIFO policy
        ret = pthread_setschedparam(this_thread, SCHED_FIFO, &params);
        if (ret != 0) {
            // Unsuccessful in setting thread realtime priority
            return false;
        }
        // Now verify the change in thread priority
        int policy = 0;
        ret = pthread_getschedparam(this_thread, &policy, &params);
        if (ret != 0) {
            // Couldn't retrieve real-time scheduling paramers
            return false;
        }
        // Check the correct policy was applied
        if(policy != SCHED_FIFO) {
            // Scheduling is NOT SCHED_FIFO!
            return false;
        }
        else {
            // Success
            return true;
        }
    #endif
}

bool disable_realtime() {
    #ifdef _WIN32
        DWORD dwError;
        if (!SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS)) {
            dwError = GetLastError();
            LOG(Error) << "Failed to elevate process priority. Code: " << static_cast<int>(dwError);
            return false;
        }
        if (!SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL)) {
            dwError = GetLastError();
            LOG(Error) << "ERROR: Failed to elevate thread priority. Code: " << static_cast<int>(dwError);
            return false;
        }
        return true;
    #else
        int ret;
        // We'll operate on the currently running thread.
        pthread_t this_thread = pthread_self();
        // struct sched_param is used to store the scheduling priority
        struct sched_param params;
        // We'll set the priority to the maximum.
        params.sched_priority = sched_get_priority_max(SCHED_OTHER);
         // Attempt to set thread normal priority to the SCHED_OTHER policy
        ret = pthread_setschedparam(this_thread, SCHED_OTHER, &params);
        if (ret != 0) {
            // Unsuccessful in setting thread normal priority
            return false;
        }
        // Now verify the change in thread priority
        int policy = 0;
        ret = pthread_getschedparam(this_thread, &policy, &params);
        if (ret != 0) {
            // Couldn't retrieve normal scheduling paramers
            return false;
        }
        // Check the correct policy was applied
        if(policy != SCHED_OTHER) {
            // Scheduling is NOT SCHED_OTHER!
            return false;
        }
        else {
            // Success
            return true;
        }
    #endif
}

uint32 get_thread_id() {
    #ifdef _WIN32
    return GetCurrentThreadId();
    #elif defined(__linux__)
    return static_cast<unsigned int>(::syscall(__NR_gettid));
    #elif defined(__APPLE__)
    uint64_t tid64;
    pthread_threadid_np(NULL, &tid64);
    return static_cast<unsigned int>(tid64);
    #endif
}

#ifdef _WIN32

struct VersionGetter
{
    VersionGetter()
    {
        const auto system = L"kernel32.dll";
        DWORD dummy;
        const auto cbInfo =
            ::GetFileVersionInfoSizeExW(FILE_VER_GET_NEUTRAL, system, &dummy);
        std::vector<char> buffer(cbInfo);
        ::GetFileVersionInfoExW(FILE_VER_GET_NEUTRAL, system, dummy, (DWORD)buffer.size(), &buffer[0]);
        void *p = nullptr;
        UINT size = 0;
        ::VerQueryValueW(buffer.data(), L"\\", &p, &size);
        assert(size >= sizeof(VS_FIXEDFILEINFO));
        assert(p != nullptr);
        auto pFixed = static_cast<const VS_FIXEDFILEINFO *>(p);
        std::stringstream ss;
        ss << HIWORD(pFixed->dwFileVersionMS) << '.'
           << LOWORD(pFixed->dwFileVersionMS) << '.'
           << HIWORD(pFixed->dwFileVersionLS) << '.'
           << LOWORD(pFixed->dwFileVersionLS) << '\n';
        ver = ss.str();
    }
    
    std::string ver;
};

const std::string& os_name() {
    static std::string name = "Windows";
    return name;
}

const std::string& os_version()
{
    static VersionGetter getter;
    return getter.ver;
}

#elif (__APPLE__)

struct NameGetter {
    NameGetter() {
        FILE* stdoutFile = popen("sw_vers -productName","r");
        if (stdoutFile) {
            char buff[32];
            char* stdout = fgets(buff, sizeof(buff), stdoutFile);
            name = stdout;
            if (!name.empty() && name[name.length()-1] == '\n')
                name.erase(name.length()-1);
            pclose(stdoutFile);
        }
    }
    std::string name = "N/A";
};

struct VersionGetter {
    VersionGetter() {
        FILE* stdoutFile = popen("sw_vers -productVersion","r");
        if (stdoutFile) {
            char buff[32];
            char* stdout = fgets(buff, sizeof(buff), stdoutFile);
            ver = stdout;
            if (!ver.empty() && ver[ver.length()-1] == '\n')
                ver.erase(ver.length()-1);
            pclose(stdoutFile);
        }
    }
    std::string ver = "";
};

const std::string& os_name() {
    static NameGetter getter;
    return getter.name;
}

const std::string& os_version() {
    static VersionGetter getter;
    return getter.ver;
}

#else

const std::string& os_name() {
    static std::string name = "UNKNOWN"; // TODO 
    return name;
}

const std::string& os_version() {
    static std::string ver = "UNKNOWN"; // TODO
    return ver;
}

#endif

//==============================================================================
// PERFORMANCE MONITORING FUNCTIONS (WINDOWS)
//==============================================================================

#ifdef _WIN32

// for CPU usage total
static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

// for CPU usage process
static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

// We need to initialize a few things for Windows functions, so we create a
// simple class with the the init code in its constructor and create an isntance
struct PerformanceInitializer {
    PerformanceInitializer() {
        // for CPU usage total
        PdhOpenQuery(0, 0, &cpuQuery);
        PdhAddCounter(cpuQuery, "\\Processor(_Total)\\% Processor Time", 0, &cpuTotal);
        PdhCollectQueryData(cpuQuery);
        // for CPU usage process
        SYSTEM_INFO sysInfo;
        FILETIME ftime, fsys, fuser;

        GetSystemInfo(&sysInfo);
        numProcessors = sysInfo.dwNumberOfProcessors;

        GetSystemTimeAsFileTime(&ftime);
        memcpy(&lastCPU, &ftime, sizeof(FILETIME));

        self = GetCurrentProcess();
        GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
        memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
        memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
    }
    ~PerformanceInitializer() { }
};

// create an instance, calls the init code in constructor
PerformanceInitializer global_initializer;

double cpu_usage_total() {
    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal);
    return counterVal.doubleValue;
}

double cpu_usage_process() {
    FILETIME ftime, fsys, fuser;
    ULARGE_INTEGER now, sys, user;
    double percent;

    GetSystemTimeAsFileTime(&ftime);
    memcpy(&now, &ftime, sizeof(FILETIME));

    GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
    memcpy(&sys, &fsys, sizeof(FILETIME));
    memcpy(&user, &fuser, sizeof(FILETIME));
    percent = (double)((sys.QuadPart - lastSysCPU.QuadPart) +
        (user.QuadPart - lastUserCPU.QuadPart));
    percent /= (now.QuadPart - lastCPU.QuadPart);
    percent /= numProcessors;
    lastCPU = now;
    lastUserCPU = user;
    lastSysCPU = sys;

    return percent * 100;
}

uint64 virt_mem_available() {
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPageFile;
}

uint64 virt_mem_used_total() {
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPageFile - mem_info.ullAvailPageFile;
}

uint64 virt_mem_used_process() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.PrivateUsage;
}

uint64 ram_available() {
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPhys;
}

uint64 ram_used_total() {
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&mem_info);
    return mem_info.ullTotalPhys - mem_info.ullAvailPhys;
}

uint64 ram_used_process() {
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

#else // TODO

double cpu_usage_total() { return 0;}
double cpu_usage_process() { return 0;}
uint64 virt_mem_available() { return 0;}
uint64 virt_mem_used_total() { return 0;}
uint64 virt_mem_used_process() { return 0;}
uint64 ram_available() { return 0;}
uint64 ram_used_total() { return 0;}
uint64 ram_used_process() { return 0;}

#endif

} // namespace util
} // namespace mahi
