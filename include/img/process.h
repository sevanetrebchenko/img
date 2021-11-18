
#ifndef IMG_PROCESS_H
#define IMG_PROCESS_H

#include "img.h"
#include "img/image.h"

namespace img {

    class process {
        public:
            explicit process(const std::string& filepath);
            explicit process(const image& im);
            ~process();

            [[nodiscard]] image get() const;

            // Utility functions.
            [[nodiscard]] process& to_grayscale();
            [[nodiscard]] process& to_lower_resolution(int rows = 1, int columns = 1);

            // Image processing functions.
            // Convert the image to ascii characters.
            //   resolution - pixels per letter
            [[nodiscard]] std::string to_ascii(int resolution = 20);

            // Convert the image via the k-means clustering algorithm.
            //   k - number of clusters
            [[nodiscard]] process& k_means(int k);

        private:
            [[nodiscard]] std::string get_output_directory() const;

            // Get the average color in a subsection of the given image.
            [[nodiscard]] glm::vec4 get_average_color(int x, int y, int rows, int columns) const;

            image im;
    };

}

#endif //IMG_PROCESS_H
