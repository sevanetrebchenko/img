
#ifndef IMG_IMAGE_H
#define IMG_IMAGE_H

#include "img.h"
#include "img/pixel.h"
#include "img/file_data.h"

namespace img {

    // Forward declaration.
    class processor;

    class image {
        public:
            explicit image(const std::string& filepath);
            image(const std::string& filename, int width, int height, int channels);
            ~image();

            image(const image& other);
            image& operator=(const image& other);

            [[nodiscard]] pixel get_pixel(int x, int y) const;
            void set_pixel(int x, int y, const pixel& value);
            void set_pixel(int x, int y, const glm::vec4& value);

            void save() const;

            [[nodiscard]] processor process() const;

            [[nodiscard]] int get_height() const;
            [[nodiscard]] int get_width() const;
            [[nodiscard]] int get_channels() const;

        private:
            friend class processor;

            file_data file;
            int width;
            int height;
            int channels;
            int total;
            unsigned char* data;

            bool stb_allocated;
    };

}

#endif //IMG_IMAGE_H
