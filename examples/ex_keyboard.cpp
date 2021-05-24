#include <Mahi/Util.hpp>

using namespace mahi::util;

// See JSON for Modern C++ for more examples:
// https://github.com/nlohmann/json

int main(int argc, char const *argv[])
{   
    // little setup for printing every second as we wait 
    Clock clock;
    int last_print = 0;

    std::cout << "Press the key \"P\" to print to screen." << std::endl;
    std::cout << "Press the key \"C\" to continue." << std::endl;

    bool pressed = false;
    while(!pressed){
        // tries to get a keypress, if there was none, ch is set to 0
        int ch = get_key_nb();
        // by default, these are lower case, so we must compare against 'p' and 'c'. You can use toupper to make it uppercase
        if(ch == 'p') std::cout << "P key was pressed." << std::endl;
        if(ch == 'c') pressed = true;
        // prints every second to show that we are not stuck at get_key_nb!
        if((int)clock.get_elapsed_time().as_seconds() != last_print){
            std::cout << "get_key_nb() is nonblocking so other things can happen when this is in a loop" << std::endl;
            last_print += 1;
        }
    }

    std::cout << "get_key() is blocking, so now you must press \"Enter\" or \"Esc\" before continuing this example" << std::endl;
    // Enter and KEY_ESCAPE are 0 and 1, so we will initialize ch to -1 so that we don't automatically hit the case
    int ch = -1;
    while(ch != KEY_ENTER && ch != KEY_ESCAPE) {
        // waits until we get a keypress and then assigns that key value to ch
        ch = get_key();
        // if the user doesn't press k, tell them they made a mistake, and tell them to press K!
        if(ch != KEY_ENTER && ch != KEY_ESCAPE) std::cout << "Silly you, you have to press \"Enter\" or \"Esc\", not " << (unsigned char)toupper(ch) << std::endl;
    }
    
    
    std::cout << "Finally, to end this example, you can press any key." << std::endl;    
    std::cout << "We check this using kb_hit()." << std::endl;
    while(!kb_hit()){}

    // this can be used to clear the input buffer. I am not sure if it is totally safe yet, so I am not putting it into the api
    while (get_key_nb() != 0);

    return 0;
}
