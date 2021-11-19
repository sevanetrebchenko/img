
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

            // Utility functions.
            [[nodiscard]] processor& to_grayscale();
            [[nodiscard]] processor& to_lower_resolution(int x_resolution = 1, int y_resolution = 1);

            // Image processing functions.
            // Convert the image to ascii characters.
            //   resolution - pixels per letter
            [[nodiscard]] std::string to_ascii(int resolution = 20);

            // Convert the image via the k-means clustering algorithm.
            //   k - number of clusters
            [[nodiscard]] processor& k_means(int k);

        private:
            image im;
    };

}

#endif //IMG_PROCESSOR_H
