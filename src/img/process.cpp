
#include "img/process.h"
#include "img/utility.h"

namespace img {

    process::process(const std::string &filepath) : im(filepath) {
    }

    process::process(const image &im) : im(im) {
    }

    process::~process() = default;

    image process::get() const {
        return im;
    }

    process &process::to_grayscale() {
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

    process &process::to_lower_resolution(int rows, int columns) {
        int width = im.width;
        int height = im.height;
        int channels = im.channels;


        // Invalid dimensions.
        if (rows < 0 || columns < 0) {
            std::cerr << "Invalid image dimensions passed to to_lower_resolution()." << std::endl;
            return *this;
        }

        for (int y = 0; y <= height - columns; y += columns) {
            for (int x = 0; x <= width - rows; x += rows) {
                // Fill pixel data to be the same color.
                for (int j = 0; j < columns; ++j) {
                    for (int i = 0; i < rows; ++i) {
                        im.set_pixel(x + i, y + j, get_average_color(x, y, rows, columns));
                    }
                }
            }
        }

        // In case resolution didn't divide evenly.
        int x_remainder = width % rows;
        int y_remainder = height % columns;

        // Right side of image.
        for (int y = 0; y < height - columns; y += columns) {
            // Fill pixel data to be the same color.
            for (int j = 0; j < columns; ++j) {
                for (int i = 0; i < x_remainder; ++i) {
                    im.set_pixel(width - x_remainder + i, y + j,
                                 get_average_color(width - x_remainder, y, rows, columns));
                }
            }
        }

        // Bottom side of image.
        for (int x = 0; x < width - rows; x += rows) {
            // Fill pixel data to be the same color.
            for (int j = 0; j < y_remainder; ++j) {
                for (int i = 0; i < rows; ++i) {
                    im.set_pixel(x + i, height - y_remainder + j,
                                 get_average_color(x, height - y_remainder, rows, columns));
                }
            }
        }

        // Bottom right corner.
        for (int j = 0; j < y_remainder; ++j) {
            for (int i = 0; i < x_remainder; ++i) {
                im.set_pixel(width - x_remainder + i, height - y_remainder + j,
                             get_average_color(width - x_remainder, height - y_remainder, rows, columns));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + std::to_string(rows) + 'x' +
                               std::to_string(columns) + "px" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    std::string process::to_ascii(int resolution) {
        int width = im.width;
        int height = im.height;

        // TODO: user-defined sequence.
        std::vector<char> sequence{'@', '%', '#', '*', '+', '=', '-', ':', '.'};
        std::size_t num_characters = sequence.size();
        std::string ascii;

        process copy = *this;

        image lower_resolution_image = process(im).to_lower_resolution(resolution, resolution).get();

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

    process &process::k_means(int k) {
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

    std::string process::get_output_directory() const {
        static std::string output_directory = convert_to_native_separators("assets/generated");
        return output_directory; // Copy.
    }

    glm::vec4 process::get_average_color(int x, int y, int rows, int columns) const {
        glm::vec4 data(0.0f);

        // Average pixels in one "pixel".
        for (int j = 0; j < columns; ++j) {
            for (int i = 0; i < rows; ++i) {
                data += im.get_pixel(x + i, y + j);
            }
        }
        data /= (float) (rows * columns);

        return data;
    }

}
