
#ifndef IMG_UTILITY_H
#define IMG_UTILITY_H

#include "pch.h"

namespace img {

    std::string convert_to_native_separators(std::string path);

    std::string get_asset_name(const std::string& path);
    std::string get_asset_extension(const std::string& path);

}

#endif //IMG_UTILITY_H
