
#include "img/processor.h"
#include "img/utility.h"

namespace img {

    processor::processor(const image &im) : im(im) {
    }

    processor::~processor() = default;

    image processor::get() const {
        return im;
    }

    void processor::save() const {
        im.save();
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

        // Invalid dimensions.
        if (x_resolution < 0 || y_resolution < 0) {
            std::cerr << "Invalid image dimensions passed to to_lower_resolution()." << std::endl;
            return *this;
        }

        glm::vec4 data;

        for (int y = 0; y <= height - y_resolution; y += y_resolution) {
            for (int x = 0; x <= width - x_resolution; x += x_resolution) {
                data = get_average_color(x, y, x_resolution, y_resolution);

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
            data = get_average_color(width - x_remainder, y, x_remainder, y_resolution);

            // Fill pixel data to be the same color.
            for (int j = 0; j < y_resolution; ++j) {
                for (int i = 0; i < x_remainder; ++i) {
                    im.set_pixel(width - x_remainder + i, y + j, data);
                }
            }
        }

        // Bottom side of image.
        for (int x = 0; x < width - x_resolution; x += x_resolution) {
            data = get_average_color(x, height - y_remainder, x_resolution, y_remainder);

            // Fill pixel data to be the same color.
            for (int j = 0; j < y_remainder; ++j) {
                for (int i = 0; i < x_resolution; ++i) {
                    im.set_pixel(x + i, height - y_remainder + j, data);
                }
            }
        }

        // Bottom right corner.
        data = get_average_color(width - x_remainder, height - y_remainder, x_remainder, y_remainder);

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

    processor &processor::k_means(int k, bool maintain_alpha) {
        int width = im.width;
        int height = im.height;

        // pixel mapped to ID of centroid.
        std::vector<int> cluster_ids;
        cluster_ids.resize(width * height, -1);

        // Get all unique colors.
        std::unordered_set<glm::vec3> unique_colors;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                unique_colors.emplace(im.get_pixel(x, y));
            }
        }

        // Generate k random centroids.
        std::vector<glm::vec3> centroids;
        for (int i = 0; i < k; ++i) {
            int index = random(0, static_cast<int>(unique_colors.size()));
            centroids.emplace_back(*std::next(unique_colors.begin(), index));
        }

        // Run k-means clustering.
        while (assign_cluster_ids(centroids, cluster_ids)) {
            update_centroids(centroids, cluster_ids);
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;

                int cluster_id = cluster_ids[index];
                assert(cluster_id >= 0); // Check cluster ID validity.
                const glm::vec3& color = centroids[cluster_id];

                // Update color.
                unsigned alpha = 255;
                if (maintain_alpha) {
                    alpha = im.get_pixel(x, y).a;
                }

                im.set_pixel(x, y, glm::vec4(color, alpha));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "k_means" + '_' + std::to_string(k) + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_error_diffusion() {
        glm::vec4 next_pixel_error = glm::vec4(0.0f);
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        glm::vec4 value;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.

                // Diffuse error.
                value += next_pixel_error;

                // For next iteration.
                next_pixel_error = skew_direction(value);
                dithered[index] = value;
            }

            // Discard error after processing one line.
            next_pixel_error = glm::vec4(0.0f);
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dithered_error_diffusion" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_floyd_steinberg() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        glm::vec4 pixel_error = glm::vec4(0.0f);

        // Floyd-Steinberg requires storage for the error values being applied on the next row.
        std::vector<glm::vec4> row_error;
        row_error.resize(width, glm::vec4(0.0f));

        std::vector<glm::vec4> next_row_error;
        next_row_error.resize(width, glm::vec4(0.0f));

        glm::vec4 value;

        // Floyd-Steinberg dithering matrix:
        //           X    7/16      <- next pixel error
        //    3/16  5/16  1/16      <- next row error (accumulated)
        float next = 7.0f / 16.0f;
        float row1 = 3.0f / 16.0f;
        float row2 = 5.0f / 16.0f;
        float row3 = 1.0f / 16.0f;

        for (int y = 0; y < height - 1; ++y) {
            // Ensure valid bounds.
            for (int x = 1; x < width - 1; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.

                // Apply error from previous pixel in current row and diffused error from the above row.
                value += (pixel_error + row_error[x]);

                glm::vec4 unscaled_error = skew_direction(value);
                dithered[index] = value;

                // Error calculations.
                pixel_error = next * unscaled_error;
                next_row_error[x - 1] += row1 * unscaled_error;
                next_row_error[x + 0] += row2 * unscaled_error;
                next_row_error[x + 1] += row3 * unscaled_error;
            }

            // Discard next pixel error after processing one line.
            pixel_error = glm::vec4(0.0f);

            // Update next row error to apply to the next row.
            row_error = next_row_error;
            for (int i = 0; i < width; ++i) {
                next_row_error[i] = glm::vec4(0.0f);
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dithered_floyd_steinberg" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_false_floyd_steinberg() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        glm::vec4 pixel_error = glm::vec4(0.0f);

        // Floyd-Steinberg requires storage for the error values being applied on the next row.
        std::vector<glm::vec4> row_error;
        row_error.resize(width, glm::vec4(0.0f));

        std::vector<glm::vec4> next_row_error;
        next_row_error.resize(width, glm::vec4(0.0f));

        glm::vec4 value;

        // Floyd-Steinberg dithering matrix:
        //   X   3/8      <- next pixel error
        //  3/8  2/8      <- next row error (accumulated)
        float next = 3.0f / 8.0f;
        float row1 = 3.0f / 8.0f;
        float row2 = 2.0f / 8.0f;

        for (int y = 0; y < height - 1; ++y) {
            // Ensure valid bounds.
            for (int x = 0; x < width - 1; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.

                // Apply error from previous pixel in current row and diffused error from the above row.
                value += (pixel_error + row_error[x]);

                glm::vec4 unscaled_error = skew_direction(value);
                dithered[index] = value;

                // Error calculations.
                pixel_error = next * unscaled_error;
                next_row_error[x + 0] += row1 * unscaled_error;
                next_row_error[x + 1] += row2 * unscaled_error;
            }

            // Discard next pixel error after processing one line.
            pixel_error = glm::vec4(0.0f);

            // Update next row error to apply to the next row.
            row_error = next_row_error;
            for (int i = 0; i < width; ++i) {
                next_row_error[i] = glm::vec4(0.0f);
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dithered_false_floyd_steinberg" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_jarvis_judice_ninke() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        // Algorithm requires storage for the error values being applied on the two next rows.
        std::vector<glm::vec4> error;
        error.resize(width * height, glm::vec4(0.0f));

        // Jarvis-Judice-Ninke dithering matrix:
        //               X    7/48  5/48
        //  3/48  5/48  7/48  5/48  3/48
        //  1/48  3/48  5/48  3/48  1/48
        float error1 = 1.0f / 48.0f;
        float error3 = 3.0f / 48.0f;
        float error5 = 5.0f / 48.0f;
        float error7 = 7.0f / 48.0f;

        glm::vec4 value;

        for (int y = 0; y < height - 2; ++y) {
            // Ensure valid bounds.
            for (int x = 2; x < width - 2; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.
                value += error[index];

                glm::vec4 unscaled_error = skew_direction(value);

                // Propagate error to neighboring cells based on error propagation matrix.
                error[(x + 1) + width * (y + 0)] += error7 * unscaled_error;
                error[(x + 2) + width * (y + 0)] += error5 * unscaled_error;

                error[(x - 2) + width * (y + 1)] += error3 * unscaled_error;
                error[(x - 1) + width * (y + 1)] += error5 * unscaled_error;
                error[(x - 0) + width * (y + 1)] += error7 * unscaled_error;
                error[(x + 1) + width * (y + 1)] += error5 * unscaled_error;
                error[(x + 2) + width * (y + 1)] += error3 * unscaled_error;

                error[(x - 2) + width * (y + 2)] += error1 * unscaled_error;
                error[(x - 1) + width * (y + 2)] += error3 * unscaled_error;
                error[(x - 0) + width * (y + 2)] += error5 * unscaled_error;
                error[(x + 1) + width * (y + 2)] += error3 * unscaled_error;
                error[(x + 2) + width * (y + 2)] += error1 * unscaled_error;

                dithered[index] = value;
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dithered_jarvis_judice_ninke" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_stucki() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        // Algorithm requires storage for the error values being applied on the two next rows.
        std::vector<glm::vec4> error;
        error.resize(width * height, glm::vec4(0.0f));

        // Stucki dithering matrix:
        //               X    8/42  4/42
        //  2/42  4/42  8/42  4/42  2/42
        //  1/42  2/42  4/42  2/42  1/42
        float error1 = 1.0f / 42.0f;
        float error2 = 2.0f / 42.0f;
        float error4 = 4.0f / 42.0f;
        float error8 = 8.0f / 42.0f;

        glm::vec4 value;

        for (int y = 0; y < height - 2; ++y) {
            // Ensure valid bounds.
            for (int x = 2; x < width - 2; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.
                value += error[index];

                glm::vec4 unscaled_error = skew_direction(value);

                // Propagate error to neighboring cells based on error propagation matrix.
                error[(x + 1) + width * (y + 0)] += error8 * unscaled_error;
                error[(x + 2) + width * (y + 0)] += error4 * unscaled_error;

                error[(x - 2) + width * (y + 1)] += error2 * unscaled_error;
                error[(x - 1) + width * (y + 1)] += error4 * unscaled_error;
                error[(x - 0) + width * (y + 1)] += error8 * unscaled_error;
                error[(x + 1) + width * (y + 1)] += error4 * unscaled_error;
                error[(x + 2) + width * (y + 1)] += error2 * unscaled_error;

                error[(x - 2) + width * (y + 2)] += error1 * unscaled_error;
                error[(x - 1) + width * (y + 2)] += error2 * unscaled_error;
                error[(x - 0) + width * (y + 2)] += error4 * unscaled_error;
                error[(x + 1) + width * (y + 2)] += error2 * unscaled_error;
                error[(x + 2) + width * (y + 2)] += error1 * unscaled_error;

                dithered[index] = value;
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dither_stucki" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_atkinson() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        // Algorithm requires storage for the error values being applied on the two next rows.
        std::vector<glm::vec4> error;
        error.resize(width * height, glm::vec4(0.0f));

        // Stucki dithering matrix:
        //        X   1/8  1/8
        //  1/8  1/8  1/8
        //       1/8
        float error1 = 1.0f / 8.0f;

        glm::vec4 value;

        for (int y = 0; y < height - 2; ++y) {
            // Ensure valid bounds.
            for (int x = 2; x < width - 2; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.
                value += error[index];

                glm::vec4 unscaled_error = skew_direction(value);

                // Propagate error to neighboring cells based on error propagation matrix.
                error[(x + 1) + width * (y + 0)] += error1 * unscaled_error;
                error[(x + 2) + width * (y + 0)] += error1 * unscaled_error;

                error[(x - 1) + width * (y + 1)] += error1 * unscaled_error;
                error[(x - 0) + width * (y + 1)] += error1 * unscaled_error;
                error[(x + 1) + width * (y + 1)] += error1 * unscaled_error;

                error[(x - 0) + width * (y + 2)] += error1 * unscaled_error;

                dithered[index] = value;
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dither_atkinson" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_burkes() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        // Algorithm requires storage for the error values being applied on the two next rows.
        std::vector<glm::vec4> error;
        error.resize(width * height, glm::vec4(0.0f));

        // Burkes dithering matrix:
        //               X    8/32  4/32
        //  2/32  4/32  8/32  4/32  2/32
        float error2 = 2.0f / 32.0f;
        float error4 = 4.0f / 32.0f;
        float error8 = 8.0f / 32.0f;

        glm::vec4 value;

        for (int y = 0; y < height - 2; ++y) {
            // Ensure valid bounds.
            for (int x = 2; x < width - 2; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.
                value += error[index];

                glm::vec4 unscaled_error = skew_direction(value);

                // Propagate error to neighboring cells based on error propagation matrix.
                error[(x + 1) + width * (y + 0)] += error8 * unscaled_error;
                error[(x + 2) + width * (y + 0)] += error4 * unscaled_error;

                error[(x - 2) + width * (y + 1)] += error2 * unscaled_error;
                error[(x - 1) + width * (y + 1)] += error4 * unscaled_error;
                error[(x - 0) + width * (y + 1)] += error8 * unscaled_error;
                error[(x + 1) + width * (y + 1)] += error4 * unscaled_error;
                error[(x + 2) + width * (y + 1)] += error2 * unscaled_error;

                dithered[index] = value;
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dither_burkes" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_sierra() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        // Algorithm requires storage for the error values being applied on the two next rows.
        std::vector<glm::vec4> error;
        error.resize(width * height, glm::vec4(0.0f));

        // Sierra dithering matrix:
        //               X    5/32  3/32
        //  2/32  4/32  5/32  4/32  2/32
        //        2/32  3/32  2/32
        float error2 = 2.0f / 32.0f;
        float error3 = 3.0f / 32.0f;
        float error4 = 4.0f / 32.0f;
        float error5 = 5.0f / 32.0f;

        glm::vec4 value;

        for (int y = 0; y < height - 2; ++y) {
            // Ensure valid bounds.
            for (int x = 2; x < width - 2; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.
                value += error[index];

                glm::vec4 unscaled_error = skew_direction(value);

                // Propagate error to neighboring cells based on error propagation matrix.
                error[(x + 1) + width * (y + 0)] += error5 * unscaled_error;
                error[(x + 2) + width * (y + 0)] += error3 * unscaled_error;

                error[(x - 2) + width * (y + 1)] += error2 * unscaled_error;
                error[(x - 1) + width * (y + 1)] += error4 * unscaled_error;
                error[(x - 0) + width * (y + 1)] += error5 * unscaled_error;
                error[(x + 1) + width * (y + 1)] += error4 * unscaled_error;
                error[(x + 2) + width * (y + 1)] += error2 * unscaled_error;

                error[(x - 1) + width * (y + 2)] += error2 * unscaled_error;
                error[(x - 0) + width * (y + 2)] += error3 * unscaled_error;
                error[(x + 1) + width * (y + 2)] += error2 * unscaled_error;

                dithered[index] = value;
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dither_sierra" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_two_row_sierra() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        // Algorithm requires storage for the error values being applied on the two next rows.
        std::vector<glm::vec4> error;
        error.resize(width * height, glm::vec4(0.0f));

        // Two-Row Sierra dithering matrix:
        //               X    4/16  3/16
        //  1/16  2/16  3/16  2/16  1/16
        float error1 = 1.0f / 16.0f;
        float error2 = 2.0f / 16.0f;
        float error3 = 3.0f / 16.0f;
        float error4 = 4.0f / 16.0f;

        glm::vec4 value;

        for (int y = 0; y < height - 2; ++y) {
            // Ensure valid bounds.
            for (int x = 2; x < width - 2; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.
                value += error[index];

                glm::vec4 unscaled_error = skew_direction(value);

                // Propagate error to neighboring cells based on error propagation matrix.
                error[(x + 1) + width * (y + 0)] += error4 * unscaled_error;
                error[(x + 2) + width * (y + 0)] += error3 * unscaled_error;

                error[(x - 2) + width * (y + 1)] += error1 * unscaled_error;
                error[(x - 1) + width * (y + 1)] += error2 * unscaled_error;
                error[(x - 0) + width * (y + 1)] += error3 * unscaled_error;
                error[(x + 1) + width * (y + 1)] += error2 * unscaled_error;
                error[(x + 2) + width * (y + 1)] += error1 * unscaled_error;

                dithered[index] = value;
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dither_two_row_sierra" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    processor &processor::dither_sierra_lite() {
        int height = im.height;
        int width = im.width;

        std::vector<glm::vec4> dithered;
        dithered.resize(width * height, glm::vec4(0.0f));

        // Algorithm requires storage for the error values being applied on the two next rows.
        std::vector<glm::vec4> error;
        error.resize(width * height, glm::vec4(0.0f));

        // Sierra-Lite dithering matrix:
        //               X   2/4
        //        1/4   1/4
        float error1 = 1.0f / 4.0f;
        float error2 = 2.0f / 4.0f;

        glm::vec4 value;

        for (int y = 0; y < height - 2; ++y) {
            // Ensure valid bounds.
            for (int x = 2; x < width - 2; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.
                value += error[index];

                glm::vec4 unscaled_error = skew_direction(value);

                // Propagate error to neighboring cells based on error propagation matrix.
                error[(x + 1) + width * (y + 0)] += error2 * unscaled_error;

                error[(x - 1) + width * (y + 1)] += error1 * unscaled_error;
                error[(x - 0) + width * (y + 1)] += error1 * unscaled_error;

                dithered[index] = value;
            }
        }

        // Write resulting colors back to image.
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                int index = x + width * y;
                im.set_pixel(x, y, glm::vec4(glm::vec3(dithered[index]), 255.0f));
            }
        }

        // Update naming.
        std::string filename = get_output_directory() + "/" + im.file.name + '_' + "dither_sierra_lite" + '.' + im.file.extension;
        im.file = file_data(filename);

        return *this;
    }

    std::string processor::get_output_directory() const { // NOLINT(readability-convert-member-functions-to-static)
        static std::string output_directory = convert_to_native_separators("assets/generated/" + im.file.name);
        return output_directory; // Copy.
    }

    glm::vec4 processor::get_average_color(int x, int y, int x_resolution, int y_resolution) const {
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

    float processor::euclidian_distance(const glm::vec3& first, const glm::vec3& second) const { // NOLINT(readability-convert-member-functions-to-static)
        return glm::length2(second - first); // Magnitude squared.
    }

    bool processor::assign_cluster_ids(const std::vector<glm::vec3>& centroids, std::vector<int>& cluster_ids) const {
        int height = im.height;
        int width = im.width;
        bool needs_updating = false;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                glm::vec3 data = glm::vec3(im.get_pixel(x, y));

                int index = x + width * y;
                int cluster_id = -1;
                float smallest_distance = std::numeric_limits<float>::infinity();

                // Get the closest centroid to this pixel.
                for (int i = 0; i < centroids.size(); ++i) {
                    float current_distance = euclidian_distance(data, centroids[i]);

                    if (current_distance < smallest_distance) {
                        // Found closer centroid.
                        smallest_distance = current_distance;
                        cluster_id = i; // Update cluster index.
                    }
                }

                if (cluster_id != cluster_ids[index]) {
                    // Cluster changed, centroids need to be updated.
                    cluster_ids[index] = cluster_id;
                    needs_updating = true;
                }
            }
        }

        return needs_updating;
    }

    void processor::update_centroids(std::vector<glm::vec3>& centroids, const std::vector<int>& cluster_ids) const {
        int height = im.height;
        int width = im.width;

        for (int i = 0; i < centroids.size(); ++i) {
            glm::vec3 sum(0.0f);
            int num_elements = 0;

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    int index = x + width * y;
                    if (cluster_ids[index] == i) {
                        // Do not consider colors with opacity to be a different color.
                        pixel pix = im.get_pixel(x, y);
                        sum += glm::vec3(pix);
                        ++num_elements;
                    }
                }
            }

            // Centroid needs updating.
            if (num_elements > 0) {
                centroids[i] = sum / static_cast<float>(num_elements);
            }
        }
    }

    glm::vec4 processor::skew_direction(glm::vec4& value) const {
        static const glm::vec4 zero = glm::vec4(0.0f);
        static const glm::vec4 one = glm::vec4(255.0f);

        float distance_to_zero = euclidian_distance(value, zero);
        float distance_to_one = euclidian_distance(value, one);

        // Set to zero or one depending on which one the given 'value' is closest to.
        // Returns distance to the closest value.
        glm::vec4 distance;

        if (distance_to_zero < distance_to_one) {
            distance = value - zero;
            value = zero;
        }
        else {
            distance = value - one;
            value = one;
        }

        return distance;
    }

}
