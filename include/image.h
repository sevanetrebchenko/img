
#ifndef ITOA_IMAGE_H
#define ITOA_IMAGE_H

#include <string>
#include <glm/glm.hpp>

namespace img {

    class image {
        public:
            struct pixel_data {
                explicit pixel_data(unsigned char r = 0x00, unsigned char g = 0x00, unsigned char b = 0x00, unsigned char a = 0xff);

                // Convert pixel data to glm::vec4.
                explicit operator glm::vec4() const;

                unsigned char r;
                unsigned char g;
                unsigned char b;
                unsigned char a;
            };


            explicit image(const std::string& filename);
            image(const std::string& filename, int width, int height, int channels);
            ~image();

            [[nodiscard]] pixel_data get_pixel(int x, int y) const;
            void set_pixel(int x, int y, pixel_data data);

            void save() const;

            // Copy this image into a grayscale version.
            [[nodiscard]] image to_grayscale() const;
            [[nodiscard]] image to_lower_resolution(int x_resolution = 1, int y_resolution = 1) const;

            void to_ascii() const;

        private:
            constexpr static const float red_weight = 0.299f;
            constexpr static const float green_weight = 0.587f;
            constexpr static const float blue_weight = 0.114f;

            int _width;
            int _height;
            int _channels;
            std::string _filename;
            bool _stb_allocated;
            unsigned char* _data;
    };

}

#endif //ITOA_IMAGE_H
