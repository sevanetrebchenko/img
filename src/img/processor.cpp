
#include "img/processor.h"
#include "img/utility.h"

namespace img {

    // Global functions.
    [[nodiscard]] inline std::string get_output_directory() {
        static std::string output_directory = convert_to_native_separators("assets/generated");
        return output_directory; // Copy.
    }

    [[nodiscard]] inline glm::vec4 get_average_color(const image& im, int x, int y, int x_resolution, int y_resolution) {
        glm::vec4 data(0.0f);

        // Average values in one "pixel".
        for (int j = 0; j < y_resolution; ++j) {
            for (int i = 0; i < x_resolution; ++i) {
                data += im.get_pixel(x + i, y + j);
            }
        }
        data /= (float) (x_resolution * y_resolution);

        return data;
    }


    processor::processor(const image &im) : im(im) {
    }

    processor::~processor() = default;

    image processor::get() const {
        return im;
    }

    processor &processor::to_grayscale() {
        // Luminosity constants.
        static const float r_weight = 0.299f;
        static const float g_weight = 0.587f;
        static const float b_weight = 0.114f;

        int width = im.width;
        int height = im.height;
        int channels = im.channels;

        // Convert to grayscale.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                // Scale pixel value by grayscale modifiers.
                glm::vec4 value = glm::vec4(im.get_pixel(x, y));
                unsigned char color = static_cast<unsigned char>((float) value.r * r_weight +
                                                                 (float) value.g * g_weight +
                                                                 (float) value.b * b_weight);
                im.set_pixel(x, y, glm::vec4(color, color, color, 1.0f));
            }
        }

        // Update name.
        std::string filename = get_output_directory() + "/" + im.file.name + "_grayscale." + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::to_lower_resolution(int x_resolution, int y_resolution) {
        int width = im.width;
        int height = im.height;
        int channels = im.channels;

        // Invalid dimensions.
        if (x_resolution < 0 || y_resolution < 0) {
            std::cerr << "Invalid image dimensions passed to to_lower_resolution()." << std::endl;
            return *this;
        }

        glm::vec4 data;

        for (int y = 0; y <= height - y_resolution; y += y_resolution) {
            for (int x = 0; x <= width - x_resolution; x += x_resolution) {
                data = get_average_color(im, x, y, x_resolution, y_resolution);

                // Fill pixel data to be the same color.
                for (int j = 0; j < y_resolution; ++j) {
                    for (int i = 0; i < x_resolution; ++i) {
                        im.set_pixel(x + i, y + j, data);
                    }
                }
            }
        }

        // In case resolution didn't divide evenly.
        int x_remainder = width % x_resolution;
        int y_remainder = height % y_resolution;

        // Right side of image.
        for (int y = 0; y < height - y_resolution; y += y_resolution) {
            data = get_average_color(im, width - x_remainder, y, x_remainder, y_resolution);

            // Fill pixel data to be the same color.
            for (int j = 0; j < y_resolution; ++j) {
                for (int i = 0; i < x_remainder; ++i) {
                    im.set_pixel(width - x_remainder + i, y + j, data);
                }
            }
        }

        // Bottom side of image.
        for (int x = 0; x < width - x_resolution; x += x_resolution) {
            data = get_average_color(im, x, height - y_remainder, x_resolution, y_remainder);

            // Fill pixel data to be the same color.
            for (int j = 0; j < y_remainder; ++j) {
                for (int i = 0; i < x_resolution; ++i) {
                    im.set_pixel(x + i, height - y_remainder + j, data);
                }
            }
        }

        // Bottom right corner.
        data = get_average_color(im, width - x_remainder, height - y_remainder, x_remainder, y_remainder);

        // Fill pixel data to be the same color.
        for (int j = 0; j < y_remainder; ++j) {
            for (int i = 0; i < x_remainder; ++i) {
                im.set_pixel(width - x_remainder + i, height - y_remainder + j, data);
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + std::to_string(x_resolution) + 'x' + std::to_string(y_resolution) + "px" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    std::string processor::to_ascii(int resolution) {
        int width = im.width;
        int height = im.height;

        // TODO: user-defined sequence.
        std::vector<char> sequence{'@', '%', '#', '*', '+', '=', '-', ':', '.'};
        std::size_t num_characters = sequence.size();
        std::string ascii;

        processor copy = *this;

        image lower_resolution_image = processor(im).to_lower_resolution(resolution, resolution).get();

        for (int y = 0; y < height; y += 2 * resolution) {
            for (int x = 0; x < width; x += resolution) {
                pixel pixel = lower_resolution_image.get_pixel(x, y);
                ascii += sequence[static_cast<int>((static_cast<float>(pixel.r) / 255.0f *
                                                    static_cast<float>(num_characters)))];
            }

            ascii += '\n';
        }

        return ascii;
    }

    processor &processor::k_means(int k) {
        int width = im.width;
        int height = im.height;

        // Get all unique colors.
        std::unordered_set<glm::vec4> unique_colors;

        for (int y = 0; y <= height; ++y) {
            for (int x = 0; x <= width; ++x) {
                unique_colors.emplace(glm::vec4(glm::vec3(im.get_pixel(x, y)), 1.0f));
            }
        }

        // Generate k random centroids.
        std::vector<glm::vec4> centroids;
        for (int i = 0; i < k; ++i) {
            int index = random(0, static_cast<int>(unique_colors.size()));
            centroids.emplace_back(*std::next(unique_colors.begin(), index));
        }
    }

}
