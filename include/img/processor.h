
#ifndef IMG_PROCESSOR_H
#define IMG_PROCESSOR_H

#include "img.h"
#include "image.h"

namespace img {

    // Processor stores its own image, all processes operate on the local copy.
    // Allows for chaining of image processing.
    class processor {
        public:
            // Deep copies image, original image passed is not modified.
            explicit processor(const image& im);
            ~processor();

            // Creates copy.
            [[nodiscard]] image get() const;
            void save() const;

            // Utility functions.
            [[nodiscard]] processor& to_grayscale();
            [[nodiscard]] processor& to_lower_resolution(int x_resolution = 1, int y_resolution = 1);

            // Image processing functions.
            // Convert the image to ascii characters.
            //   resolution - pixels per letter
            [[nodiscard]] std::string to_ascii(int resolution = 20);

            // Convert the image via the k-means clustering algorithm.
            //   k - number of clusters
            [[nodiscard]] processor& k_means(int k, bool maintain_alpha = false);

            // Dithering algorithms.
            [[nodiscard]] processor& dither_error_diffusion();
            [[nodiscard]] processor& dither_floyd_steinberg();
            [[nodiscard]] processor& dither_false_floyd_steinberg();
            [[nodiscard]] processor& dither_jarvis_judice_ninke();
            [[nodiscard]] processor& dither_stucki();
            [[nodiscard]] processor& dither_atkinson();
            [[nodiscard]] processor& dither_burkes();
            [[nodiscard]] processor& dither_sierra();
            [[nodiscard]] processor& dither_two_row_sierra();
            [[nodiscard]] processor& dither_sierra_lite();
            [[nodiscard]] processor& dither_bayer(int matrix_width, int matrix_height);

        private:
            [[nodiscard]] std::string get_output_directory() const;

            // Samples pixels in a x_resolution by y_resolution area, starting at (x, y) from the given image.
            // Returns the average color of the pixels.
            [[nodiscard]] glm::vec4 get_average_color(int x, int y, int x_resolution, int y_resolution) const;


            // K-Means clustering algorithm helper functions.
            // Returns squared euclidian distance between two given points.
            [[nodiscard]] float euclidian_distance(const glm::vec3& first, const glm::vec3& second) const;

            // Updates each cluster ID with the ID of the closest centroid.
            [[nodiscard]] bool assign_cluster_ids(const std::vector<glm::vec3>& centroids, std::vector<int>& cluster_ids) const;

            // Updates the centroid values based on the surrounding cluster data.
            void update_centroids(std::vector<glm::vec3>& centroids, const std::vector<int>& cluster_ids) const;


            // Dithering helper functions.
            [[nodiscard]] glm::vec4 skew_direction(glm::vec4& value) const;


            // Constructs ordered dithering threshold map with side length as a power of 2. Resulting matrix is
            // normalized.
            // n-values corresponding to matrix dimensions:
            //   0 - 2x2
            //   1 - 4x4
            //   2 - 8x8
            //   3 - 16x16
            [[nodiscard]] std::vector<float> get_bayer_matrix(int n) const;

            // Constructs un-normalized threshold map based on https://en.wikipedia.org/wiki/Ordered_dithering#Pre-calculated_threshold_maps.
            [[nodiscard]] std::vector<float> get_bayer_matrix_helper(int n) const;

            // Converts from n-value to Bayer Matrix dimension.
            [[nodiscard]] int get_bayer_matrix_dimension(int n) const;

            image im;
    };

}

#endif //IMG_PROCESSOR_H
