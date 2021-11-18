
#include "img/file_data.h"
#include "img/utility.h"

namespace img {

    file_data::file_data(const std::string &filepath) : path(convert_to_native_separators(filepath)),
                                                        directory(get_directory(path)),
                                                        name(get_asset_name(path)),
                                                        extension(get_asset_extension(path)) {
    }

    file_data::~file_data() = default;

}
