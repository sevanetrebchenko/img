
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
        grid.resize(grid_size * grid_size);

        // Initialize values to a default -1.
        for (int i = 0; i < grid_size; ++i) {
            for (int j = 0; j < grid_size; ++j) {
                grid[j + grid_size * i] = -1;
            }
        }

        std::vector<int> active_list;
        std::vector<glm::ivec2> point_list;

        // Generate initial random point.
        glm::vec2 random (uniform_distribution(0.0f, static_cast<float>(grid_size)), uniform_distribution(0.0f, static_cast<float>(grid_size)));
        glm::ivec2 initial_point(glm::floor(random));

        int valid_index = 0;

        active_list.emplace_back(valid_index++);
        point_list.emplace_back(initial_point);

        while (!active_list.empty()) {

            // Choose random index.
            int index = active_list[uniform_distribution(0, (int)active_list.size() - 1)];
            glm::vec2 point = point_list[index];

            bool valid = false;

            for (int i = 0; i < rejection_limit; ++i) {
                // Generate point uniformly in spherical annulus between radius r and 2r around the chosen point.
                float angle = uniform_distribution(0.0f, 3.14159f * 2.0f);
                float radius = uniform_distribution(0.0f, 1.0f) * minimum_separation + minimum_separation;
                glm::vec2 offset (radius * glm::cos(angle), radius * glm::sin(angle));

                glm::ivec2 test(glm::floor((point + offset) / cell_size));

                if (test.x < 0 || test.x > grid_size) {
                    continue;
                }

                if (test.y < 0 || test.y > grid_size) {
                    continue;
                }

                // Check to make sure selected grid location doesn't already have a point.
                if (grid[test.x + grid_size * test.y] != -1) {
                    continue;
                }

                bool passed = true;

                for (int y = -1; y < 2; ++y) {
                    for (int x = -1; x < 2; ++x) {
                        // Don't check against test cell.
                        if (x == 0 && y == 0) {
                            continue;
                        }

                        glm::ivec2 position(test.x + x, test.y + y);

                        // Validate cell bounds.
                        if (position.x < 0 || position.x >= grid_size) {
                            continue;
                        }

                        if (position.y < 0 || position.y >= grid_size) {
                            continue;
                        }

                        int grid_index = grid[position.x + grid_size * position.y];
                        if (grid_index >= 0) {
                            // Found point at this index, compare distances.
                            if (glm::distance(glm::vec2(position), glm::vec2(test)) < minimum_separation) {
                                // Point too close.
                                passed = false;
                            }
                        }
                    }
                }

                if (passed) {
                    valid = true;

                    grid[test.x + grid_size * test.y] = valid_index;
                    active_list.emplace_back(valid_index++);
                    point_list.emplace_back(test);
                    break;
                }
            }

            if (!valid && !active_list.empty()) {
                active_list.erase(active_list.begin() + index);
            }
        }

        return point_list;
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
