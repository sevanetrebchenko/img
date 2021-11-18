
#ifndef IMG_IMAGE_H
#define IMG_IMAGE_H

#include <string>
#include <glm/glm.hpp>

#include "pixel.h"

namespace img {

    class image {
        public:
            explicit image(const std::string& filename);
            image(const std::string& filename, int width, int height, int channels);
            ~image();

            [[nodiscard]] pixel_data get_pixel(int x, int y) const;
            void set_pixel(int x, int y, pixel data);

            void save() const;
            void save(const std::string& output_directory) const;

            // Copy this image into a grayscale version.
            [[nodiscard]] image to_grayscale() const;
            [[nodiscard]] image to_lower_resolution(int rows = 1, int columns = 1) const;

            void to_ascii(int resolution = 20) const;

        private:
            constexpr static const float red_weight = 0.299f;
            constexpr static const float green_weight = 0.587f;
            constexpr static const float blue_weight = 0.114f;

            std::string _base;

            int _width;
            int _height;
            int _channels;
            std::string _filename;
            std::string _extension;
            bool _stb_allocated;
            unsigned char* _data;
    };

}

#endif //IMG_IMAGE_H
