#include <image_manips.hpp>
#include <iostream>

int main(int argc, char** argv) {
    if (argc < 7) {
        std::cout << "usage: ./my_image_processor_lvl1 <input_file> <output_file>"
                  << std::endl;

        return 1;
    }


    // vvvvv Use this code if using only one input image:
    // std::string input_file_path  = argv[1];
    // std::string output_file_path = argv[2];
    // Image       input_image1      = read_image(input_file_path);

    // vvvvv Use this code if using two input image:
    std::string input_file_path1  = argv[1];
    std::string input_file_path2 = argv[2];
    std::string process_file_path1 = argv[3];
    std::string process_file_path2 = argv[4];
    std::string process_file_path3 = argv[5];
    std::string process_file_path4 = argv[6];
    std::string output_file_path = argv[7];
    Image input_image1 = read_image(input_file_path1);
    Image input_image2 = read_image(input_file_path2);
    Image process_image1 = read_image(process_file_path1);
    Image process_image2 = read_image(process_file_path2);
    Image process_image3 = read_image(process_file_path3);
    Image process_image4 = read_image(process_file_path4);

    // You can use more than two input images if you want !

    // Be creative !
    // vvv Write code here vvv
    // crop_image(input_image1, 550, 89, 339, 99);
    Image swap =
    swap_channels(input_image1, Channel::Blue, Channel::Green);
    Image process1 =
    superposition(swap, input_image2, 20, 70);
    Image process2 =
    superposition(process1, process_image1, 100, 200);
    Image process3 =
    superposition(process2, process_image2, 400, 300);
    Image process4 =
    superposition(process3, process_image3, 530, 67);
    Image result =
    superposition(process4, process_image4, 670, 520);   
    

    // Uncomment the following line and save the correct image
    write_image(output_file_path, result);

    return 0;
}