
#include <utility.h>
#include <string>
#include <algorithm>

namespace img {

    bool compare_file_extension(const std::string& filename, const std::string& extension) {
        return get_asset_extension(filename) == extension;
    }

    void convert_to_native_separators(std::string& path) {
    #ifdef _WIN32
        const char separator = '/';
        const char native = '\\';
    #else
        const char separator = '\\';
        const char native = '/';
    #endif
        std::replace(path.begin(), path.end(), separator, native);
    }

    std::string get_asset_name(const std::string& filepath) {
        // Get the last slash position.
        std::size_t slashPosition = std::string::npos;
        std::size_t winslash = filepath.find_last_of('\\');
        std::size_t slash = filepath.find_last_of('/');

        if (winslash != std::string::npos) {
            slashPosition = winslash;
        }

        if (slash != std::string::npos) {
            if (slash > winslash) {
                slashPosition = slash;
            }
        }

        // If there exists a slash.
        std::string assetName = filepath;

        if (slashPosition != std::string::npos) {
            assetName = filepath.substr(slashPosition + 1);
        }

        // Take off extension.
        std::size_t dot = assetName.find_last_of('.');
        if (dot != std::string::npos) {
            return assetName.substr(0, dot);
        }

        return assetName;
    }

    std::string get_asset_extension(const std::string& path) {
        std::size_t dot = path.find_last_of('.');
        if (dot != std::string::npos) {
            return path.substr(dot + 1);
        }

        return path;
    }

}
