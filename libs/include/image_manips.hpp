/**
 * @file image_manips.hpp
 * @author GRY Nicolas (nicolas.gry@inria.fr)
 * @brief
 * @version 0.1
 * @date 2026-08-05
 *
 * @copyright Copyright (c) 2026
 *
 * @details
 */
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#if defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__) || \
    defined(__MINGW32__) || defined(__BORLANDC__)
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

/**
 * @brief Decribes image color channels
 * 
 */
enum class Channel { Red, Green, Blue };

/**
 * @brief The image type. 
 * The layout is: image[color channel][y coordinate][x coordinate]
 * 
 */
using Image = std::vector<std::vector<std::vector<uint8_t>>>;

/**
 * @brief Returns the channel corresponding to the word stored in the argument
 * channel.
 *
 * @param channel
 * @return
 */
EXPORT
Channel channel_from_str(const std::string channel);

/**
 * @brief Returns the width of the provided image
 *
 * @param image
 * @return int that is the number of pixel width of the image. Returns -1 if image is empty.
 */
EXPORT
int get_image_width(const Image& image);

/**
 * @brief Returns the height of the provided image
 *
 * @param image
 * @return int that is the number of pixel height of the image. Returns -1 if
 * image is empty.
 */
EXPORT
int get_image_height(const Image& image);

/**
 * @brief Reads an RGB image from a file.
 *
 * @param filename Path to the image file.
 * @return An Image containing the pixel data (R, G, B).
 */
EXPORT
Image read_image(const std::string filename);

/**
 * @brief Writes an RGB image to a PNG file.
 *
 * @param filename Path to save the image.
 * @param image The Image object containing the data.
 */
EXPORT
void write_image(const std::string filename, const Image& image);

/**
 * @brief Swaps the values between two specified color channels in an RGB image.
 *
 * Modifies the input image structure by swapping all R/G/B values
 * for every pixel between c1 and c2. The input image isn't modified in placed.
 *
 * @param image The Image to modify.
 * @param c1 The first channel to swap.
 * @param c2 The second channel to swap.
 * @return An Image containing the pixel data .
 */
EXPORT
Image swap_channels(const Image& image, Channel c1, Channel c2);

/**
 * @brief Converts an Image to a BWImage by averaging the R, G, and B
 * channels.
 *
 * Each pixel in the resulting BWImage holds the arithmetic mean of its RGB
 * components.
 *
 * @param rgb_image The input RGB image.
 * @return A new BWImage containing the calculated grayscale data.
 */
EXPORT
void convert_rgb_to_bw(Image& rgb_image);

/**
 * @brief Create a blank image of size width x height and filled with the color
 * (red, green, blue). Color is black by default.
 *
 * @param width
 * @param height
 * @param red
 * @param green
 * @param blue
 * @return
 */
EXPORT
Image create_blank_image(const int width,
                         const int height,
                         const int red   = 0,
                         const int green = 0,
                         const int blue  = 0);

enum class ConcatMode { Horizontal, Vertical };

/**
 * @brief Concatenate image1 and image2 in a new image that is returned by the
 * function. The provided images are not modified.
 *
 * The concatenate mode determines whether to put the images side by side
 * (Vertical) or above one another (Horizontal).
 *
 * Vertical mode: if the images have different widths, the smaller image is
 * padded with black background.
 *
 * Horizontal mode: if the images have different heights, the smaller image is
 * padded with black background.
 *
 * @param image1
 * @param image2
 * @param mode
 * @return EXPORT
 */
EXPORT
Image concatenate(const Image& image1, const Image& image2, ConcatMode mode);

/**
 * @brief Crops in place to the rectangle located at (top_left_x, top_left_y) of dimension width x height. 
 * 
 * The rectangle is clipped to the image bounds; if nothing remains, the image becomes empty.
 *
 * @param image_to_crop
 * @param top_left_x
 * @param top_left_y
 * @param width
 * @param height
 * @return EXPORT
 */
EXPORT
void crop_image(Image& image_to_crop,
                int    top_left_x,
                int    top_left_y,
                int    width,
                int    height);

/**
 * @brief Returns a copy of background with top_image drawn on top of it.
 * 
 * (top_image_x, top_image_y) is the position of top_image's top-left corner in background coordinates (may be negative or partly outside; the overflow is clipped). The result has the same size as the background.
 * @param background
 * @param top_image
 * @param top_image_x
 * @param top_image_y
 * @return EXPORT
 */
EXPORT
Image superposition(const Image& background,
                    const Image& top_image,
                    int          top_image_x,
                    int          top_image_y);

/**
 * @brief Rotates the image by 90 degrees in place (clockwise or
 * counter-clockwise).
 *
 * @param image
 * @param clockwise
 * @return EXPORT
 */
EXPORT
void rotate90(Image& image, bool clockwise);