#include <Mahi/Util/Console.hpp>
#include <Mahi/Util/Concurrency/Mutex.hpp>
#include <csignal>
#include <iostream>
#include <cstdio>
#include <fmt/format.h>

#ifdef _WIN32
#include <io.h>
#include <windows.h>
#define _NO_OLDNAMES  // for MinGW compatibility
#include <conio.h>    // for getch() and kbhit()
#define getch _getch
#define kbhit _kbhit
#else
#include <termios.h>   // for getch() and kbhit()
#include <unistd.h>    // for getch(), kbhit() and (u)sleep()
#include <sys/ioctl.h> // for getkey()
#include <sys/types.h> // for kbhit()
#include <sys/time.h>  // for kbhit()
#endif

namespace mahi {
namespace util {

//==============================================================================
// CONSTANTS
//==============================================================================

namespace {

#ifdef _WIN32
HANDLE stdout_handle = GetStdHandle(static_cast<DWORD>(-11));
CONSOLE_SCREEN_BUFFER_INFO g_csbiInfo;
struct Initializer {
    Initializer() {
        GetConsoleScreenBufferInfo(stdout_handle, &g_csbiInfo);
    }
    ~Initializer() { SetConsoleTextAttribute(stdout_handle, g_csbiInfo.wAttributes); }
};
Initializer g_initializer;
#endif

// mutex to make console output thread safe
static Mutex g_console_mutex;

} // namespace

//==============================================================================
// SIGNAL HANDLING
//==============================================================================

bool (*ctrl_handler)(CtrlEvent);

#ifdef _WIN32
BOOL win_handler(DWORD var) {
    if (var == CTRL_C_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::CtrlC));
    else if (var == CTRL_BREAK_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::CtrlQuit));
    else if (var == CTRL_CLOSE_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::Close));
    else if (var == CTRL_LOGOFF_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::Logoff));
    else if (var == CTRL_SHUTDOWN_EVENT)
        return static_cast<int>(ctrl_handler(CtrlEvent::Shutdown));
    else
        return 0;
}

bool register_ctrl_handler(bool (*handler)(CtrlEvent)) {
    ctrl_handler = handler;
    return !!SetConsoleCtrlHandler((PHANDLER_ROUTINE)win_handler, TRUE);
}
#else

void posix_handler(int sig) {
    if (sig == SIGINT)
        ctrl_handler(CtrlEvent::CtrlC);
    else if (sig == SIGQUIT)
        ctrl_handler(CtrlEvent::CtrlQuit);
}

bool register_ctrl_handler(bool (*handler)(CtrlEvent)) {
    ctrl_handler = handler;
    signal(SIGINT, posix_handler);
    signal(SIGQUIT, posix_handler);
    return true;
}

#endif

//==============================================================================
// CONSOLE FORMAT (THREAD SAFE)
//==============================================================================

#ifdef _WIN32

WORD get_color(ConsoleColor color, bool background) {
    DWORD val = 0;
    switch(color) {

        case ConsoleColor::None:
            if (background)
                return (g_csbiInfo.wAttributes & ~(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY));
            val = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            break;

        case ConsoleColor::Black:
            break;

        case ConsoleColor::White:
            val = FOREGROUND_INTENSITY;
        case ConsoleColor::Gray:
            val = val | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case ConsoleColor::Red:
            val = FOREGROUND_INTENSITY;
        case ConsoleColor::DarkRed:
            val = val | FOREGROUND_RED;
            break;

        case ConsoleColor::Green:
            val = FOREGROUND_INTENSITY;
        case ConsoleColor::DarkGreen:
            val = val | FOREGROUND_GREEN;
            break;

        case ConsoleColor::Blue:
            val = FOREGROUND_INTENSITY;
        case ConsoleColor::DarkBlue:
            val = val | FOREGROUND_BLUE;
            break;

        case ConsoleColor::Cyan:
            val = FOREGROUND_INTENSITY;
        case ConsoleColor::Aqua:
            val = val | FOREGROUND_GREEN | FOREGROUND_BLUE;
            break;

        case ConsoleColor::Magenta:
            val = FOREGROUND_INTENSITY;
        case ConsoleColor::Purple:
            val = val | FOREGROUND_RED | FOREGROUND_BLUE;
            break;

        case ConsoleColor::Yellow:
            val = FOREGROUND_INTENSITY;
        case ConsoleColor::Gold:
            val = val | FOREGROUND_RED | FOREGROUND_GREEN;
            break;

    }
    if (background)
        return static_cast<WORD>(val * 16);
    else
        return static_cast<WORD>(val);
}

void set_text_color(ConsoleColor foreground, ConsoleColor background) {
    WORD attributes = get_color(foreground, false) | get_color(background, true);
    Lock lock(g_console_mutex);
    SetConsoleTextAttribute(stdout_handle, attributes);
}

void reset_text_color() {
    Lock lock(g_console_mutex);
    SetConsoleTextAttribute(stdout_handle, g_csbiInfo.wAttributes);
}

struct VirtualTerminalInit {
    VirtualTerminalInit() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD consoleMode;
        GetConsoleMode(hConsole, &consoleMode);
        initMode = consoleMode;
        consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hConsole, consoleMode);
    }
    ~VirtualTerminalInit() {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleMode(hConsole, initMode);
    }
    DWORD initMode;
};

void enable_virtual_console() {
    static VirtualTerminalInit init;
}

#else

void set_text_color(ConsoleColor foreground, ConsoleColor background) {
    Lock lock(g_console_mutex);
    // background
    if (background == ConsoleColor::None)
        std::cout << "\x1B[0m";
    else if (background == ConsoleColor::Black)
        std::cout << "\x1B[40m";
    else if (background == ConsoleColor::Gray)
        std::cout << "\x1B[100m";
    else if (background == ConsoleColor::White)
        std::cout << "\x1B[107m";
    else if (background == ConsoleColor::Red)
        std::cout << "\x1B[101m";
    else if (background == ConsoleColor::DarkRed)
        std::cout << "\x1B[41m";
    else if (background == ConsoleColor::Green)
        std::cout << "\x1B[102m";
    else if (background == ConsoleColor::DarkGreen)
        std::cout << "\x1B[42m";
    else if (background == ConsoleColor::Blue)
        std::cout << "\x1B[104m";
    else if (background == ConsoleColor::DarkBlue)
        std::cout << "\x1B[44m";
    else if (background == ConsoleColor::Cyan)
        std::cout << "\x1B[106m";
    else if (background == ConsoleColor::Aqua)
        std::cout << "\x1B[46m";
    else if (background == ConsoleColor::Magenta)
        std::cout << "\x1B[105m";
    else if (background == ConsoleColor::Purple)
        std::cout << "\x1B[45m";
    else if (background == ConsoleColor::Yellow)
        std::cout << "\x1B[103m";
    else if (background == ConsoleColor::Gold)
        std::cout << "\x1B[43m";
    // set foreground
    if (foreground == ConsoleColor::None)
        std::cout << "\x1B[97m";
    else if (foreground == ConsoleColor::Black)
        std::cout << "\x1B[30m";
    else if (foreground == ConsoleColor::Gray)
        std::cout << "\x1B[90m";
    else if (foreground == ConsoleColor::White)
        std::cout << "\x1B[97m";
    else if (foreground == ConsoleColor::Red)
        std::cout << "\x1B[91m";
    else if (foreground == ConsoleColor::DarkRed)
        std::cout << "\x1B[31m";
    else if (foreground == ConsoleColor::Green)
        std::cout << "\x1B[92m";
    else if (foreground == ConsoleColor::DarkGreen)
        std::cout << "\x1B[32m";
    else if (foreground == ConsoleColor::Blue)
        std::cout << "\x1B[94m";
    else if (foreground == ConsoleColor::DarkBlue)
        std::cout << "\x1B[34m";
    else if (foreground == ConsoleColor::Cyan)
        std::cout << "\x1B[96m";
    else if (foreground == ConsoleColor::Aqua)
        std::cout << "\x1B[36m";
    else if (foreground == ConsoleColor::Magenta)
        std::cout << "\x1B[95m";
    else if (foreground == ConsoleColor::Purple)
        std::cout << "\x1B[35m";
    else if (foreground == ConsoleColor::Yellow)
        std::cout << "\x1B[93m";
    else if (foreground == ConsoleColor::Gold)
        std::cout << "\x1B[33m";
}

void reset_text_color() {
    Lock lock(g_console_mutex);
    std::cout << "\x1B[0m\x1B[0K";
}

void enable_virtual_console() {
    // DO NOTHING FOR NOW
}

#endif

//==============================================================================
// CONSOLE INPUT (THREAD SAFE)
//==============================================================================

#ifndef _WIN32
/// Get character without waiting for Return to be pressed.
/// Windows has this in conio.h
int getch(void) {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

/// Determines if keyboard has been hit.
/// Windows has this in conio.h
int kbhit(void) {
    static struct termios oldt, newt;
    int cnt = 0;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag    &= ~(ICANON | ECHO);
    newt.c_iflag     = 0; // input mode
    newt.c_oflag     = 0; // output mode
    newt.c_cc[VMIN]  = 1; // minimum time to wait
    newt.c_cc[VTIME] = 1; // minimum characters to wait for
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ioctl(0, FIONREAD, &cnt); // Read count
    struct timeval tv;
    tv.tv_sec  = 0;
    tv.tv_usec = 100;
    select(STDIN_FILENO+1, NULL, NULL, NULL, &tv); // A small time delay
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return cnt; // Return number of characters
}
#endif

int kb_hit() {
    Lock lock(g_console_mutex);
    return kbhit();
}

int get_ch() {
    Lock lock(g_console_mutex);
    return getch();
}

int get_ch_nb() {
    Lock lock(g_console_mutex);
    if (kbhit())
        return getch();
    else
        return 0;
}

int get_key(void) {
    Lock lock(g_console_mutex);
    #ifndef _WIN32
    int cnt = kbhit(); // for ANSI escapes processing
    #endif
    int k = getch();
    switch(k) {
        case 0: {
            int kk;
            switch (kk = getch()) {
                case 71: return KEY_HOME;
                case 72: return KEY_UP;
                case 73: return KEY_PGUP;
                case 75: return KEY_LEFT;
                case 77: return KEY_RIGHT;
                case 79: return KEY_END;
                case 80: return KEY_DOWN;
                case 81: return KEY_PGDOWN;
                case 82: return KEY_INSERT;
                case 83: return KEY_DELETE;
                default: return kk-59 + KEY_F1; // Function keys
            }}
        case 224: {
            int kk;
            switch (kk = getch()) {
                case 71: return KEY_HOME;
                case 72: return KEY_UP;
                case 73: return KEY_PGUP;
                case 75: return KEY_LEFT;
                case 77: return KEY_RIGHT;
                case 79: return KEY_END;
                case 80: return KEY_DOWN;
                case 81: return KEY_PGDOWN;
                case 82: return KEY_INSERT;
                case 83: return KEY_DELETE;
                case 133: return KEY_F11;
                case 134: return KEY_F12;
                default: return kk; // Catch all
            }}
#ifdef __APPLE__
        case 10:  return KEY_ENTER;
        case 127: return KEY_BACKSPACE;
#else
        case 8:  return KEY_BACKSPACE;
        case 9:  return KEY_TAB;
        case 13: return KEY_ENTER;
        case 32: return KEY_SPACE;
        case 48: return KEY_NUM0;
        case 49: return KEY_NUM1;
        case 50: return KEY_NUM2;
        case 51: return KEY_NUM3;
        case 52: return KEY_NUM4;
        case 53: return KEY_NUM5;
        case 54: return KEY_NUM6;
        case 55: return KEY_NUM7;
        case 56: return KEY_NUM8;
        case 57: return KEY_NUM9;
#endif

#ifdef _WIN32
        case 27: return KEY_ESCAPE;
#else // _WIN32
        case 155: // single-character CSI
        case 27: {
            // Process ANSI escape sequences
            if (cnt >= 3 && getch() == '[') {
                switch (k = getch()) {
                    case 'A': return KEY_UP;
                    case 'B': return KEY_DOWN;
                    case 'C': return KEY_RIGHT;
                    case 'D': return KEY_LEFT;
                }
            } else return KEY_ESCAPE;
        }
#endif // _WIN32
        default: return k;
    }
}

int get_key_nb() {
    if (kbhit())
        return get_key();
    else
        return 0;
}

//==============================================================================
// MISC
//==============================================================================

#ifdef _WIN32
const bool STDOUT_IS_A_TTY = !!_isatty(_fileno(stdout));
#else
const bool STDOUT_IS_A_TTY = !!isatty(fileno(stdout));
#endif

bool is_tty() {
    return STDOUT_IS_A_TTY;
}

void beep() {
#ifdef _WIN32
    Lock lock(g_console_mutex);
    Beep(750, 250);
#endif
}

#define UTIL_PRINT(st) do { std::cout << st; } while(false)
const std::string ANSI_CLS                = "\033[2J\033[3J";
const std::string ANSI_CURSOR_HOME        = "\033[H";

void cls(void) {
    Lock lock(g_console_mutex);
#if defined(_WIN32)
    // Based on https://msdn.microsoft.com/en-us/library/windows/desktop/ms682022%28v=vs.85%29.aspx
    const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    const COORD coordScreen = {0, 0};
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    const DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, (TCHAR)' ', dwConSize, coordScreen, &cCharsWritten);

    GetConsoleScreenBufferInfo(hConsole, &csbi);
    FillConsoleOutputAttribute(hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten);

    SetConsoleCursorPosition(hConsole, coordScreen);
#else
    UTIL_PRINT(ANSI_CLS);
    UTIL_PRINT(ANSI_CURSOR_HOME);
#endif
}

} // namespace util
} // namespace mahi
