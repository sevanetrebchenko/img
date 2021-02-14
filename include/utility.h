
#ifndef IMG_UTILITY_H
#define IMG_UTILITY_H

#include <string>

namespace img {

    bool compare_file_extension(const std::string& filename, const std::string& extension);
    void convert_to_native_separators(std::string& path);

    std::string get_asset_name(const std::string& path);
    std::string get_asset_extension(const std::string& path);

}

#endif //IMG_UTILITY_H
