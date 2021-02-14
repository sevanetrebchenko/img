
#ifndef ITOA_IMAGE_H
#define ITOA_IMAGE_H

#include <string>

namespace img {

    class image {
        public:
            struct pixel_data {
                pixel_data(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

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

//            // "Standard" character ramp for grey scale pictures, black -> white.
//            //
//            //   "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\|()1{}[]?-_+~<>i!lI;:,"^`'. "
//            //A more convincing but shorter sequence for representing 10 levels of grey is
//            //   " .:-=+*#%@"
//            void ToASCII(const char* pixelFormat = "") const;
//            void Save(const char* filename = nullptr);

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
