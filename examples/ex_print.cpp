#include <Mahi/Util.hpp>
#include <vector>
#include <string>

using namespace mahi::util;

int main(int argc, char const *argv[])
{

// if you console is PowerShell, you need to enable virtual console to do styled printing
// #define POWERSHELL 
#ifdef POWERSHELL
    enable_virtual_console();
#endif
   
    // string formatting 
    print("Hello, World"); 
    print("Hello, my name is {} and I am {} years old.", "Evan", 28);
    print("Hello, my name is {1} and I am {0} years old.", 28, "Evan");
    print("Hello, my name is {name} and I am {age} years old.", "name"_a="Evan", "age"_a=28);
    std::vector<std::string> choices = {"apple", "pear", "banana"};
    print("Your can choose {}!", fmt::join(choices, " or "));

    // text styling (You may need to call enable_virtual_console(), see above)
    print(fg(fmt::color::chartreuse), "foreground=chartreuse");
    print(bg(fmt::color::red), "background=red");
    print(bg(fmt::color::blue) | fg(fmt::color::aqua), "background=blue,foreground=aqua");

    auto fancy_style = bg(fmt::color::green) | fg(fmt::color::chartreuse) | fmt::emphasis::underline | fmt::emphasis::italic | fmt::emphasis::bold;
    print(fancy_style, "fancy style");

    // print variables
    int x = 5;
    double y = 6.2;
    std::vector<float> z = {1.0f, 2.0f, 3.0f};

    print_var(x);
    print_var(y);
    print_var(z);
    print_var(x,y,z);

    return 0;
}
