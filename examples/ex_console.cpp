#include <Mahi/Util.hpp>
#include <vector>
#include <string>

// On Windows, Powershell and CMD prompt do not support ANSI escape sequences by default.
// Constructing an instance of this struct wiill enable support for this process.
#ifdef _WIN32
#include <windows.h>
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
VirtualTerminalInit g_init;
#endif

using namespace fmt::literals; // for _a
using namespace mahi::util;

int main(int argc, char const *argv[])
{

    
    // basic fmt usage
    print("Hello, World\n"); 
    print("Hello, my name is {} and I am {} years old.\n", "Evan", 28);
    print("Hello, my name is {1} and I am {0} years old.\n", 28, "Evan");
    print("Hello, my name is {name} and I am {age} years old.\n", "name"_a="Evan", "age"_a=28);

    // text styling
    print(fg(fmt::color::chartreuse), "foreground=chartreuse\n");
    print(bg(fmt::color::red), "background=red\n");
    print(bg(fmt::color::blue) | fg(fmt::color::aqua), "background=blue,foreground=aqua\n");

    auto fancy_spec = bg(fmt::color::green) | fg(fmt::color::chartreuse) | fmt::emphasis::underline | fmt::emphasis::italic | fmt::emphasis::bold;
    print(fancy_spec, "fancy style\n");

    std::vector<std::string> choices = {"apple", "pear", "banana"};
    print("Your can choose {}!", fmt::join(choices, " or "));

    return 0;
}
