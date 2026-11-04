#include <cstdint>
#include <cstdio>
#include <image_manips.hpp>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

using BWImage = std::vector<std::vector<uint8_t>>;

static Image fill_image(const Image& image,
                        const int    width,
                        const int    height,
                        int          xoffset = 0,
                        const int    yoffset = 0) {
    auto bg = create_blank_image(width, height);
    return superposition(bg, image, xoffset, yoffset);
}

static void copy_image(uint8_t* src, int width, int height, BWImage& dst) {
    for (int r = 0; r < height; ++r) {
        const size_t row = static_cast<size_t>(r) * width;

        for (int c = 0; c < width; ++c) dst[r][c] = src[row + c];
    }
}

static void copy_image(uint8_t* src, int width, int height, Image& dst) {
    constexpr int channels = 3;

    for (int r = 0; r < height; ++r) {
        const size_t row = static_cast<size_t>(r) * width;

        for (int c = 0; c < width; ++c) {
            const size_t pixel = (row + c) * channels;

            for (int ch = 0; ch < channels; ++ch)
                dst[ch][r][c] = src[pixel + ch];
        }
    }
}

template <uint8_t channels>
auto _read_image(const std::string& filename) {
    static_assert(channels == 1 || channels == 3,
                  "read_image only supports 1 or 3 channels");

    int width, height, bpp;

    uint8_t* image =
        stbi_load(filename.c_str(), &width, &height, &bpp, channels);

    if (!image) throw std::runtime_error("Failed to load image: " + filename);

    if constexpr (channels == 1) {
        BWImage bw(height, std::vector<uint8_t>(width));
        copy_image(image, width, height, bw);
        stbi_image_free(image);
        return bw;
    } else {
        Image rgb(
            channels,
            std::vector<std::vector<uint8_t>>(height,
                                              std::vector<uint8_t>(width)));

        copy_image(image, width, height, rgb);
        stbi_image_free(image);
        return rgb;
    }
}

EXPORT
Channel channel_from_str(const std::string channel) {
    Channel res = Channel::Blue;

    if (channel == "red") {
        res = Channel::Red;
    } else if (channel == "green") {
        res = Channel::Green;
    }

    return res;
}

/**
 * @brief Returns the width of the provided image
 *
 * @param image
 * @return int that is the number of pixel width of the image
 */
EXPORT
int get_image_width(const Image& image) {
    if (image.size() == 0) {
        std::cout << "Error (get_image_width): The provided image is empty"
                  << std::endl;
        return -1;
    }
    int height = image[0].size();

    if (height == 0) {
        std::cout << "Error (get_image_width): The provided image is empty"
                  << std::endl;
        return -1;
    }

    int width = image[0][0].size();
    if (width == 0) {
        std::cout << "Error (get_image_width): The provided image is empty"
                  << std::endl;
        return -1;  // Return if dimensions are zero
    }

    return width;
}

/**
 * @brief Returns the height of the provided image
 *
 * @param image
 * @return int that is the number of pixel height of the image
 */
EXPORT
int get_image_height(const Image& image) {
    if (image.size() == 0) {
        std::cout << "Error (get_image_height): The provided image is empty"
                  << std::endl;
        return -1;
    }
    int height = image[0].size();

    if (height == 0) {
        std::cout << "Error (get_image_height): The provided image is empty"
                  << std::endl;
        return -1;
    }
    return height;
}

EXPORT Image read_image(const std::string filename) {
    constexpr uint8_t channels = 3;
    return _read_image<channels>(filename);
}

/**
 * @brief Writes an RGB image (3 channels) to a PNG file.
 *
 * Extracts R, G, B components sequentially for writing using stbi_write_png.
 *
 * @param filename The path to save the image.
 * @param width Image width.
 * @param height Image height.
 * @param channel Number of channels (must be 3 for RGB).
 * @param image The Image object containing the data.
 */
EXPORT void write_image(const std::string filename, const Image& image) {
    const int channel = image.size();
    int       height  = get_image_height(image);
    int       width   = get_image_width(image);
    if (width < 0 || height < 0) {
        std::cout << "Empty image, couldn't save" << std::endl;
        return;
    }

    // We need to gather R, G, B values sequentially: R1, G1, B1, R2, G2, B2,
    // ...
    std::vector<uint8_t> flat_data;
    flat_data.reserve(width * height * channel);

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            for (int ch = 0; ch < channel; ++ch) {
                flat_data.push_back(image[ch][r][c]);
            }
        }
    }

    // Call stbi_write_png: rgb_image is the pointer to the raw data,
    // width*CHANNEL_NUM is the total number of bytes (3 * W * H).
    stbi_write_png(filename.c_str(),
                   width,
                   height,
                   channel,
                   flat_data.data(),
                   width * channel);
}

EXPORT Image swap_channels(const Image& image, Channel c1, Channel c2) {
    if (c1 == c2) {
        return image;  // No need to swap if channels are the same
    }
    Image ret = image;

    int height = get_image_height(image);
    int width  = get_image_width(image);

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            uint8_t& val1 = ret[static_cast<size_t>(c1)][r][c];
            uint8_t& val2 = ret[static_cast<size_t>(c2)][r][c];
            std::swap(val1, val2);
        }
    }

    return ret;
}

EXPORT void convert_rgb_to_bw(Image& rgb_image) {
    if (rgb_image.size() == 0) {
        std::cout << "Error (convert_rgb_to_bw): The provided image is empty"
                  << std::endl;
        return;
    }
    int height = rgb_image[0].size();

    if (height == 0) {
        std::cout << "Error (convert_rgb_to_bw): The provided image is empty"
                  << std::endl;
        return;
    }

    int width = rgb_image[0][0].size();
    if (width == 0) {
        std::cout << "Error (convert_rgb_to_bw): The provided image is empty"
                  << std::endl;
        return;  // Return if dimensions are zero
    }

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            uint8_t r_val = rgb_image[static_cast<size_t>(Channel::Red)][r][c];
            uint8_t g_val =
                rgb_image[static_cast<size_t>(Channel::Green)][r][c];
            uint8_t b_val = rgb_image[static_cast<size_t>(Channel::Blue)][r][c];

            uint8_t average = (r_val + g_val + b_val) / 3;
            for (auto channel = 0; channel < 3; channel++) {
                rgb_image[channel][r][c] = average;
            }
        }
    }
}

EXPORT
Image create_blank_image(const int width,
                         const int height,
                         const int red,
                         const int green,
                         const int blue) {
    std::vector<std::vector<uint8_t>> _red(height,
                                           std::vector<uint8_t>(width, red));
    std::vector<std::vector<uint8_t>> _green(
        height,
        std::vector<uint8_t>(width, green));
    std::vector<std::vector<uint8_t>> _blue(height,
                                            std::vector<uint8_t>(width, blue));
    return Image{_red, _green, _blue};
}

static Image concatenate_horizontal(const Image& image1, const Image& image2) {
    const auto h1 = get_image_height(image1);
    const auto h2 = get_image_height(image2);

    const auto w1     = get_image_width(image1);
    const auto w2     = get_image_width(image2);
    const auto height = std::max(h1, h2);
    const auto width  = w1 + w2;

    Image res;

    if (h1 > h2) {
        res = fill_image(image2, width, height, w1, (height - h2) / 2);
        res = superposition(res, image1, 0, 0);
    } else {
        res = fill_image(image1, width, height, 0, (height - h1) / 2);
        res = superposition(res, image2, w1, 0);
    }

    return res;
}

static Image concatenate_vertical(const Image& image1, const Image& image2) {
    const auto h1 = get_image_height(image1);
    const auto h2 = get_image_height(image2);

    const auto w1     = get_image_width(image1);
    const auto w2     = get_image_width(image2);
    const auto height = h1 + h2;
    const auto width  = std::max(w1, w2);

    Image res;

    if (w1 > w2) {
        res = fill_image(image2, width, height, (width - w2) / 2, h1);
        res = superposition(res, image1, 0, 0);
    } else {
        res = fill_image(image1, width, height, (width - w1) / 2, 0);
        res = superposition(res, image2, 0, h1);
    }

    return res;
}

/**
 * @brief Concatenate image1 and image2 in a new image that is returned by
 * the function. The provided images are not modified.
 *
 * The concatenate mode determines whether to put the images side by side
 * (Vertical) or above one another (Horizontal).
 *
 * @param image1
 * @param image2
 * @param mode
 * @return EXPORT
 */
EXPORT Image concatenate(const Image& image1,
                         const Image& image2,
                         ConcatMode   mode) {
    switch (mode) {
        case ConcatMode::Vertical:
            return concatenate_vertical(image1, image2);
            break;
        case ConcatMode::Horizontal:
            return concatenate_horizontal(image1, image2);
            break;
        default:
            return {};
            break;
    }
}

namespace {
int image_channels(const Image& img) { return static_cast<int>(img.size()); }
}  // namespace

// Crops in place to the rectangle (top_left_x, top_left_y, width, height).
// The rectangle is clipped to the image bounds; if nothing remains, the image
// becomes empty.
void crop_image(Image& image_to_crop,
                int    top_left_x,
                int    top_left_y,
                int    width,
                int    height) {
    const int img_h = get_image_height(image_to_crop);
    const int img_w = get_image_width(image_to_crop);

    const int x0 = std::max(0, top_left_x);
    const int y0 = std::max(0, top_left_y);
    const int x1 = std::min(img_w, top_left_x + width);   // exclusive
    const int y1 = std::min(img_h, top_left_y + height);  // exclusive

    if (width <= 0 || height <= 0 || x0 >= x1 || y0 >= y1) {
        image_to_crop.clear();
        return;
    }

    Image result(image_to_crop.size());
    for (size_t c = 0; c < image_to_crop.size(); ++c) {
        result[c].reserve(y1 - y0);
        for (int y = y0; y < y1; ++y) {
            result[c].emplace_back(image_to_crop[c][y].begin() + x0,
                                   image_to_crop[c][y].begin() + x1);
        }
    }
    image_to_crop = std::move(result);
}

// Returns a copy of background with top_image drawn over it.
// (top_image_x, top_image_y) is the position of top_image's top-left corner in
// background coordinates (may be negative or partly outside; the overflow is
// clipped). The result has the same size as the background.
Image superposition(const Image& background,
                    const Image& top_image,
                    int          top_image_x,
                    int          top_image_y) {
    Image result = background;

    const int bg_h  = get_image_height(background);
    const int bg_w  = get_image_width(background);
    const int top_h = get_image_height(top_image);
    const int top_w = get_image_width(top_image);

    // Overlap region in background coordinates
    const int x_start = std::max(0, top_image_x);
    const int y_start = std::max(0, top_image_y);
    const int x_end   = std::min(bg_w, top_image_x + top_w);  // exclusive
    const int y_end   = std::min(bg_h, top_image_y + top_h);  // exclusive

    const int channels =
        std::min(image_channels(background), image_channels(top_image));

    for (int c = 0; c < channels; ++c) {
        for (int y = y_start; y < y_end; ++y) {
            for (int x = x_start; x < x_end; ++x) {
                result[c][y][x] =
                    top_image[c][y - top_image_y][x - top_image_x];
            }
        }
    }
    return result;
}

// Rotates the image by 90 degrees in place (clockwise or counter-clockwise).
#include <utility>
EXPORT
void rotate90(Image& image, bool clockwise) {
    const int h = get_image_height(image);
    const int w = get_image_width(image);

    if (image.empty() || h <= 0 || w <= 0) {
        image.clear();
        return;
    }

    // Image[channel][y][x]
    // Original: height h, width w
    // Rotated:  height w, width h
    Image result(image.size(),
                 std::vector<std::vector<uint8_t>>(w, std::vector<uint8_t>(h)));

    for (size_t c = 0; c < image.size(); ++c) {
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                if (clockwise) {
                    // (x, y) -> (h - 1 - y, x)
                    result[c][x][h - 1 - y] = image[c][y][x];
                } else {
                    // (x, y) -> (y, w - 1 - x)
                    result[c][w - 1 - x][y] = image[c][y][x];
                }
            }
        }
    }

    image = std::move(result);
}