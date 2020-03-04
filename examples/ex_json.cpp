#include <Mahi/Util.hpp>

using namespace mahi::util;

// See JSON for Modern C++ for more examples:
// https://github.com/nlohmann/json

int main(int argc, char const *argv[])
{
    json j1;

    j1["a_bool"] = true;
    j1["an_int"] = 5;
    j1["a_float"] = 3.14;
    j1["a_string"] = "I'm a string!";
    j1["a_vector"] = {1,2,3,4,5};

    // save
    std::ofstream file1("my_file.json");
    if (file1.is_open())
        file1 << std::setw(4) << j1;
    file1.close();

    // load
    json j2;
    std::ifstream file2("my_file.json");
    file2 >> j2;
    file2.close();

    if (j2["a_bool"].get<bool>()) {
        std::cout << j2["a_float"].get<float>() << std::endl;
        std::cout << j2["an_int"].get<int>() << std::endl;
        std::cout << j2["a_string"].get<std::string>() << std::endl;
        std::cout << j2["a_float"].get<float>() << std::endl;
        auto v = j2["a_vector"].get<std::vector<int>>();
        for (auto& i : v)
            std::cout << i << std::endl;
    }

    return 0;
}
