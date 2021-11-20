
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

    int random(int lower, int upper) {
        typedef std::chrono::high_resolution_clock clock;
        typedef std::chrono::duration<float, std::milli> duration;

        static clock::time_point start = clock::now();
        duration elapsed = clock::now() - start;
        std::mt19937 generator(elapsed.count());
        std::uniform_int_distribution<int> distribution(lower, upper);
        return distribution(generator);
    }

}
