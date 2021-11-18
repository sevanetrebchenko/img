
#ifndef IMG_FILE_DATA_H
#define IMG_FILE_DATA_H

#include "img.h"

namespace img {

    struct file_data {
        explicit file_data(const std::string& filepath);
        ~file_data();

        std::string path;

        // Filepath broken up into separate pieces.
        std::string directory;
        std::string name;
        std::string extension;
    };

}

#endif //IMG_FILE_DATA_H
