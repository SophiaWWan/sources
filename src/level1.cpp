#include <image_manips.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "usage: ./my_image_processor_lvl1 <input_file> <output_file>"
                  << std::endl;

        return 1;
    }

    // vvvvv Use this code if using only one input image:
    std::string input_file_path  = argv[1];
    std::string output_file_path = argv[2];
    Image       input_image      = read_image(input_file_path);

    // vvvvv Use this code if using two input image:
    // std::string input_file_path1  = argv[1];
    // std::string input_file_path2 = argv[2];
    // std::string output_file_path = argv[3];
    // Image input_image1 = read_image(input_file_path1);
    // Image input_image2 = read_image(input_file_path2);

    // You can use more than two input images if you want !

    // Be creative !
    // vvv Write code here vvv

    // Uncomment the following line and save the correct image
    // write_image(output_file_path, ...);

    return 0;
}