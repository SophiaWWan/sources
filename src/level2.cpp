#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>

#include <image_manips.hpp>

void print_usage() {
    std::cout
        << "Usage:\n"
        << "  my_image_processor_lvl2.exe swap <input> <output> "
           "<channel1> <channel2>\n"
        << "  my_image_processor_lvl2.exe stripes <input1> <input2> "
           "<output> <stripe_height>\n\n"
        << "Channels: red, green, blue\n"
        << "stripe_height is the number of rows taken from each image at a "
           "time.\n";
}

bool is_color(const std::string& value) {
    return value == "red" || value == "green" || value == "blue";
}

Image combine_stripes(const Image& image1, const Image& image2, int stripe_height) {
    const int width = std::max(get_image_width(image1), get_image_width(image2));
    const int height = std::max(get_image_height(image1), get_image_height(image2));
    Image result = create_blank_image(width, height);

    for (int y = 0; y < height; ++y) {
        const Image& preferred = ((y / stripe_height) % 2 == 0) ? image1 : image2;
        const Image& fallback = (&preferred == &image1) ? image2 : image1;

        const int preferred_height = get_image_height(preferred);
        const int preferred_width = get_image_width(preferred);
        const int fallback_height = get_image_height(fallback);
        const int fallback_width = get_image_width(fallback);

        for (int x = 0; x < width; ++x) {
            const Image* source = nullptr;
            if (y < preferred_height && x < preferred_width) {
                source = &preferred;
            } else if (y < fallback_height && x < fallback_width) {
                source = &fallback;
            }

            if (source != nullptr) {
                for (int channel = 0; channel < 3; ++channel) {
                    result[channel][y][x] = (*source)[channel][y][x];
                }
            }
        }
    }

    return result;
}

int main(int argc, char** argv) {
    if (argc < 4) {
        print_usage();
        return 1;
    }

    const std::string command = argv[1];

    if (command == "swap") {
        if (argc != 6 || !is_color(argv[4]) || !is_color(argv[5])) {
            std::cout << "swap requires two valid channel names\n";
            print_usage();
            return 1;
        }

        Image input_image = read_image(argv[2]);
        Image result = swap_channels(input_image, channel_from_str(argv[4]), channel_from_str(argv[5]));
        write_image(argv[3], result);
    } else if (command == "stripes") {
        if (argc != 6) {
            print_usage();
            return 1;
        }

        int stripe_height = atoi(argv[5]);
        if (stripe_height <= 0) {
            std::cout << "stripe_height must be a positive number\n";
            return 1;
        }

        Image image1 = read_image(argv[2]);
        Image image2 = read_image(argv[3]);
        Image result = combine_stripes(image1, image2, stripe_height);
        write_image(argv[4], result);
    } else {
        std::cout << "Unknown command\n";
        print_usage();
        return 1;
    }

    // Add some other commands if you can!
    // else if (...) {}

    return 0;
}