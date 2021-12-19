
#ifndef IMG_UTILITY_H
#define IMG_UTILITY_H

#include "img.h"

namespace img {

    [[nodiscard]] std::string convert_to_native_separators(std::string path);

    [[nodiscard]] std::string get_directory(std::string path);
    [[nodiscard]] std::string get_asset_name(std::string path);
    [[nodiscard]] std::string get_asset_extension(std::string path);

    template <typename T>
    [[nodiscard]] T uniform_distribution(T min, T max) {
        return glm::linearRand(min, max);
    }

    // Implementation of the Fast Poisson Disk Sampling white paper.
    // https://www.cs.ubc.ca/~rbridson/docs/bridson-siggraph07-poissondisk.pdf
    [[nodiscard]] std::vector<glm::ivec2> poisson_disk_2d(int width, int height, float minimum_separation, int rejection_limit);

    // Assumes positive integer power.
    [[nodiscard]] int integer_power(int base, unsigned power);

    // Gets the power of 'base' that is at least 'value'.
    [[nodiscard]] int next_power_of(int base, unsigned value);

}

#endif //IMG_UTILITY_H
