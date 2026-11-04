#include <stdlib.h>

#include <image_manips.hpp>
#include <iostream>

bool is_color(const std::string word) {
    return word == "red" || word == "green" || word == "blue";
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "usage: ./my_image_processor_lvl3 <command> <input_file> "
                     "<output_file> [ args ...]"
                  << std::endl;

        return 1;
    }

    std::string command          = argv[1];
    std::string input_file_path  = argv[2];
    std::string output_file_path = argv[3];
    // std::string arg1          = argv[4];       // if you need string
    // arguments to your command int arg2                  = atoi(argv[4]); //
    // if you need int arguments to your command

    Image input_image = read_image(input_file_path);

    if (command == "my_awesome_command") {
        // vvv Write some code here vvv

    } else if (command == "annother_awesome_command") {
        // vvv Write some code here vvv

    }
    // Add some other commands if you can!
    // else if (...) {}
    else {
        std::cout << "Unknown command" << std::endl;
    }
    return 0;
}