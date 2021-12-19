
#include "img/utility.h"

namespace img {

    char get_separator() {
        #ifdef _WIN32
            return '/';
        #else
            return '\\';
        #endif
    }

    char get_native_separator() {
        #ifdef _WIN32
            return '\\';
        #else
            return '/';
        #endif
    }

    std::string convert_to_native_separators(std::string path) {
        std::replace(path.begin(), path.end(), get_separator(), get_native_separator());
        return path;
    }

    std::string get_directory(std::string path) {
        std::string directory;
        path = convert_to_native_separators(path);

        std::size_t last_separator_index = path.rfind(get_native_separator());

        if (last_separator_index != std::string::npos) {
            // Found position of separator.
            directory = path.substr(0, last_separator_index + 1); // Keep slash.
        }

        return directory;
    }

    std::string get_asset_name(std::string path) {
        std::string asset_name;
        path = convert_to_native_separators(path);

        // Substring everything after the last slash.
        std::size_t last_separator_index = path.rfind(get_native_separator());
        if (last_separator_index != std::string::npos) {
            asset_name = path.substr(last_separator_index + 1);
        }

        // Substring everything before the '.' of the extension.
        std::size_t dot = asset_name.find_last_of('.');
        if (dot != std::string::npos) {
            asset_name = asset_name.substr(0, dot);
        }

        return asset_name;
    }

    std::string get_asset_extension(std::string path) {
        std::size_t dot = path.find_last_of('.');
        if (dot != std::string::npos) {
            return path.substr(dot + 1);
        }

        return path;
    }

    std::vector<glm::ivec2> poisson_disk_2d(int width, int height, float minimum_separation, int rejection_limit) {
        float cell_size = minimum_separation / glm::sqrt(2.0f);
        int grid_width = std::ceil(static_cast<float>(width) / cell_size);
        int grid_height = std::ceil(static_cast<float>(height) / cell_size);

        int grid_size = grid_width * grid_height;

        std::vector<int> grid;
        grid.resize(grid_size);

        // Initialize values to a default -1.
        for (int i = 0; i < grid_size; ++i) {
            grid[i] = -1;
        }


        glm::ivec2 initial = uniform_distribution(glm::ivec2(0, 0), glm::ivec2(width, height));
    }

    int integer_power(int base, unsigned power) {
        int result = 1;

        if (power == 0) {
            return result;
        }

        while (power > 0) {
            result *= base;
            --power;
        }

        return result;
    }

    int next_power_of(int base, unsigned value) {
        int count = 0;

        while (value != 0) {
            value /= 2;
            ++count;
        }

        return count;
    }

}
