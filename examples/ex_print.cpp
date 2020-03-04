#include <Mahi/Util.hpp>
#include <vector>
#include <string>

using namespace fmt::literals; // for _a
using namespace mahi::util;

// if you console is PowerShell, you need to enable virtual console to do styled printing
// #define POWERSHELL 

int main(int argc, char const *argv[])
{
    
    // basic print usage
    print("Hello, World\n"); 
    print("Hello, my name is {} and I am {} years old.\n", "Evan", 28);
    print("Hello, my name is {1} and I am {0} years old.\n", 28, "Evan");
    print("Hello, my name is {name} and I am {age} years old.\n", "name"_a="Evan", "age"_a=28);

#ifdef POWERSHELL
    enable_virtual_console();
#endif

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
