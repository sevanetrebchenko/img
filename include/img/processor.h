
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

            image im;
    };

}

#endif //IMG_PROCESSOR_H
