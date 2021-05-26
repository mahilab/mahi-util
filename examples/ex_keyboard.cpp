#include <Mahi/Util/Console.hpp>
#include <Mahi/Util/Timing/Clock.hpp>

using namespace mahi::util;

std::string get_key_string(int key_press){
    switch (key_press){
        case KEY_BACKSPACE: return "BACKSPACE";
        case KEY_TAB: return "TAB";
        case KEY_ENTER: return "ENTER";
        case KEY_ESCAPE: return "ESCAPE";
        case KEY_SPACE: return "SPACE";

        case KEY_NUM0: return "NUM0";
        case KEY_NUM1: return "NUM1";
        case KEY_NUM2: return "NUM2";
        case KEY_NUM3: return "NUM3";
        case KEY_NUM4: return "NUM4";
        case KEY_NUM5: return "NUM5";
        case KEY_NUM6: return "NUM6";
        case KEY_NUM7: return "NUM7";
        case KEY_NUM8: return "NUM8";
        case KEY_NUM9: return "NUM9";

        case KEY_F1: return "F1";
        case KEY_F2: return "F2";
        case KEY_F3: return "F3";
        case KEY_F4: return "F4";
        case KEY_F5: return "F5";
        case KEY_F6: return "F6";
        case KEY_F7: return "F7";
        case KEY_F8: return "F8";
        case KEY_F9: return "F9";
        case KEY_F10: return "F10";
        case KEY_F11: return "F11";
        case KEY_F12: return "F12";

        case KEY_HOME: return "HOME";
        case KEY_UP: return "UP";
        case KEY_PGUP: return "PGUP";
        case KEY_LEFT: return "LEFT";
        case KEY_RIGHT: return "RIGHT";
        case KEY_END: return "END";
        case KEY_DOWN: return "DOWN";
        case KEY_PGDOWN: return "PGDOWN";
        case KEY_INSERT: return "INSERT";
        case KEY_DELETE: return "DELETE";
        default: {
            if (key_press >= 65 && key_press <= 122){
                int upper_int = toupper(key_press);
                return std::string((char*)&upper_int);
            }
            else{
                return "UNMAPPED KEY";
            }
        }
    }
}

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
        if(ch != KEY_ENTER && ch != KEY_ESCAPE) {
            std::string key_pressed = get_key_string(ch);
            std::cout << "Silly you, you have to press \"Enter\" or \"Esc\", not " << key_pressed << std::endl;
        }
    }
    
    
    std::cout << "Finally, to end this example, you can press any key." << std::endl;    
    std::cout << "We check this using kb_hit()." << std::endl;
    while(!kb_hit()){}

    // this can be used to clear the input buffer. I am not sure if it is totally safe yet, so I am not putting it into the api
    while (get_key_nb() != 0);

    return 0;
}
