#include <stdlib.h>

#include <image_manips.hpp>
#include <iostream>

bool is_color(const std::string word) {
    return word == "red" || word == "green" || word == "blue";
}

// My custom function: it sets a pixel to a specific color
void set_pixel(Image&  image,
               int     x,
               int     y,
               uint8_t red,
               uint8_t green,
               uint8_t blue) {
    image[0][y][x] = red;
    image[1][y][x] = green;
    image[2][y][x] = blue;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        std::cout
            << "usage: ./my_image_processor_lvl3 <tobw, set_pixels> "
               "<input_file> "
               "<output_file> [x] [y] [width] [height] [red] [green] [blue]"
            << std::endl;

        return 1;
    }

    // This program handles only two commands -> "tobw", "set_pixel"
    // It converts an input RGB image to B/W

    std::string command          = argv[1];
    std::string input_file_path  = argv[2];
    std::string output_file_path = argv[3];

    Image input_image = read_image(input_file_path);

    if (command == "tobw") {
        // An example usage of convert_rgb_to_bw
        convert_rgb_to_bw(input_image);

        write_image(output_file_path, input_image);
    } else if(command == "set_pixels") {
        // Arguments of the command:
        // set_pixel im.png out.png <x> <y> <width> <height> <red> <green> <blue>

        // The program will set the pixel in the rectangle located at (x,y), 
        // of size (width, height) to the color (red,green,blue)

        // We recover each argument value as integers
        int x = atoi(argv[4]);
        int y = atoi(argv[5]);
        int w = atoi(argv[6]);
        int h = atoi(argv[7]);
        int r = atoi(argv[8]);
        int g = atoi(argv[9]);
        int b = atoi(argv[10]);

        // We loop inside the rectangle
        for(int row = y; row < h; ++row){
            for (int col = x; col < w; ++col) {
                set_pixel(input_image, col, row, r, g, b);
            }
        }

        // We save the image
        write_image(output_file_path, input_image);
    }
    else {
        std::cout << "Unknown command" << std::endl;
    }
    return 0;
}