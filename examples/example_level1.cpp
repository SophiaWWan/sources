#include <image_manips.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 3) {
        std::cout << "usage: ./my_image_processor_lvl1 <input_file> "
                     "<input_file2> <output_file>"
                  << std::endl;

        return 1;
    }

    // I want my program to take two input image
    // So I add input_file_path2 and change the order of the program's input
    // arguments

    std::string input_file_path  = argv[1];
    std::string input_file_path2 = argv[2];
    std::string output_file_path = argv[3];

    Image input_image = read_image(input_file_path);
    Image input_image2 =
        read_image(input_file_path2);  // I also load the second image

    // An example usage of concatenate
    Image result =
        concatenate(input_image, input_image2, ConcatMode::Horizontal);

    // I save the resulting image !
    write_image(output_file_path, result);

    return 0;
}