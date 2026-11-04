#include <stdlib.h>

#include <image_manips.hpp>
#include <iostream>

bool is_color(const std::string word) {
    return word == "red" || word == "green" || word == "blue";
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout << "usage: ./my_image_processor_lvl2 <command> <input_file> "
                     "<output_file> [other arguments]"
                  << std::endl;

        return 1;
    }

    // This program handles only one command -> "tobw"
    // It converts an input RGB image to B/W

    std::string command          = argv[1];
    std::string input_file_path  = argv[2];
    std::string output_file_path = argv[3];

    Image input_image = read_image(input_file_path);

    if (command == "tobw") {
        // An example usage of convert_rgb_to_bw
        convert_rgb_to_bw(input_image);

        write_image(output_file_path, input_image);
    } else {
        std::cout << "Unknown command" << std::endl;
    }
    return 0;
}