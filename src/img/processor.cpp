
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

        glm::vec4 next_pixel_error = glm::vec4(0.0f);

        // Floyd-Steinberg requires storage for the error values being applied on the next row.
        std::vector<glm::vec4> next_row_error;
        next_row_error.resize(width, glm::vec4(0.0f));

        // Structure for building the error for the next row.
        std::vector<glm::vec4> row_error;
        row_error.resize(width, glm::vec4(0.0f));

        glm::vec4 value;

        for (int y = 0; y < height - 1; ++y) {

            for (int x = 1; x < width - 1; ++x) {
                int index = x + width * y;
                value = glm::vec4(glm::vec3(im.get_pixel(x, y)), 255.0f); // Ignore alpha channel.
                value += (next_pixel_error + next_row_error[x]);

                // For next iteration.
                glm::vec4 unscaled_error = skew_direction(value);

                // Error calculations.
                next_pixel_error = 7.0f / 16.0f * unscaled_error;

                row_error[x - 1] += 3.0f / 16.0f * unscaled_error;
                row_error[x + 0] += 5.0f / 16.0f * unscaled_error;
                row_error[x + 1] += 1.0f / 16.0f * unscaled_error;

                dithered[index] = value;
            }

            // Discard next pixel error after processing one line.
            next_pixel_error = glm::vec4(0.0f);

            // Update next row error to apply to the next row.
            next_row_error = row_error;

            for (int i = 0; i < width; ++i) {
                row_error[i] = glm::vec4(0.0f);
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
