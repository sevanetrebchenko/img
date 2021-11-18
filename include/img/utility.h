
#ifndef IMG_UTILITY_H
#define IMG_UTILITY_H

#include "img.h"

namespace img {

    std::string convert_to_native_separators(std::string path);

    std::string get_directory(std::string path);
    std::string get_asset_name(std::string path);
    std::string get_asset_extension(std::string path);

    int random(int lower, int upper);
}

#endif //IMG_UTILITY_H
