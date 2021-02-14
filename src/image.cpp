
#include <image.h>
#include <iostream>

#include <utility.h>
#include <glm/glm.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define FULL_QUALITY 100

namespace img {

    image::image(const std::string& filename) : _width(-1),
                                                _height(-1),
                                                _channels(-1),
                                                _filename(filename),
                                                _stb_allocated(true),
                                                _data(nullptr) {
        convert_to_native_separators(_filename);

//        stbi_set_flip_vertically_on_load(true);

        // Load in texture data.
        _data = stbi_load(filename.c_str(), &_width, &_height, &_channels, 0);
        if (!_data) {
            throw std::runtime_error("Failed to open file: " + std::string(filename));
        }

        // Print image information.
        std::cout << "Reading in image: " << _filename << std::endl;
        std::cout << "Width: " << _width << std::endl;
        std::cout << "Height: " << _height << std::endl;
        std::cout << "Channels: " << _channels << ' ';
        std::cout << (_channels >= 4 ? "(RGBA)" : "(RGB)") << std::endl;
    }

    image::image(const std::string& filename, int width, int height, int channels) : _width(width),
                                                                                     _height(height),
                                                                                     _channels(channels),
                                                                                     _filename(filename),
                                                                                     _stb_allocated(false),
                                                                                     _data(nullptr) {
        _data = new unsigned char[_width * _height * _channels];
    }

    image::~image() {
        _stb_allocated ? stbi_image_free(_data) : delete[] _data;
    }



    image image::to_grayscale() const {
        image grayscale(get_asset_name(_filename) + "_grayscale." + get_asset_extension(_filename), _width, _height, _channels);

        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x) {
                // Scale data by grayscale modifiers.
                pixel_data data = get_pixel(x, y);
                unsigned char color = static_cast<unsigned char>((float)data.r * red_weight + (float)data.g * green_weight + (float)data.b * blue_weight);
                pixel_data gray { color, color, color, data.a };

                grayscale.set_pixel(x, y, gray);
            }
        }

        return grayscale;
    }

    image::pixel_data image::get_pixel(int x, int y) const {
        unsigned char* offset = _data + (x + _width * y) * _channels;
        return pixel_data { offset[0], offset[1], offset[2], _channels >= 4 ? offset[3] : static_cast<unsigned char>(0xff) };
    }

    void image::set_pixel(int x, int y, pixel_data data) {
        unsigned char* offset = _data + (x + _width * y) * _channels;
        offset[0] = data.r;
        offset[1] = data.g;
        offset[2] = data.b;

        // Only set alpha if there is an alpha channel.
        if (_channels >= 4) {
            offset[3] = data.a;
        }
    }

    void image::save() const {
        if (compare_file_extension(_filename, "jpg") || compare_file_extension(_filename, "JPG") || compare_file_extension(_filename, "jpeg") || compare_file_extension(_filename, "JPEG")) {
            stbi_write_jpg(_filename.c_str(), _width, _height, _channels, _data, FULL_QUALITY);
        }
        else if (compare_file_extension(_filename, "png") || compare_file_extension(_filename, "PNG")) {
            stbi_write_png(_filename.c_str(), _width, _height, _channels, _data, _width * _channels);
        }
        else {
            std::cerr << "Invalid extension." << std::endl;
        }
    }

    image image::to_lower_resolution(int x_resolution, int y_resolution) const {
        image lower_res(get_asset_name(_filename) + "_" + std::to_string(x_resolution) + 'x' + std::to_string(y_resolution) + "px." + get_asset_extension(_filename), _width, _height, _channels);
        glm::vec4 data;

        for (int y = 0; y <= _height - y_resolution; y += y_resolution) {
            for (int x = 0; x <= _width - x_resolution; x += x_resolution) {
                data = glm::vec4(0.0f);

                // Average pixels in a x_resolution * y_resolution area.
                for (int j = 0; j < y_resolution; ++j) {
                    for (int i = 0; i < x_resolution; ++i) {
                        data += get_pixel(x + i, y + j);
                    }
                }
                data /= (float)(x_resolution * y_resolution);

                // Fill pixel data to be the same color.
                for (int j = 0; j < y_resolution; ++j) {
                    for (int i = 0; i < x_resolution; ++i) {
                        lower_res.set_pixel(x + i, y + j, pixel_data { static_cast<unsigned char>(data.r), static_cast<unsigned char>(data.g), static_cast<unsigned char>(data.b), static_cast<unsigned char>(data.a) });
                    }
                }
            }
        }

        // In case resolution didn't divide evenly.
        int x_remainder = _width % x_resolution;
        int y_remainder = _height % y_resolution;

        // Right side of image.
        for (int y = 0; y < _height - y_resolution; y += y_resolution) {
            data = glm::vec4(0.0f);

            for (int j = 0; j < y_resolution; ++j) {
                for (int i = _width - x_remainder; i < _width; ++i) {
                    data += get_pixel(i, y + j);
                }
            }
            data /= (float)(x_remainder * y_resolution);

            // Fill pixel data to be the same color.
            for (int j = 0; j < y_resolution; ++j) {
                for (int i = 0; i < x_remainder; ++i) {
                    lower_res.set_pixel(_width - x_remainder + i, y + j, pixel_data { static_cast<unsigned char>(data.r), static_cast<unsigned char>(data.g), static_cast<unsigned char>(data.b), static_cast<unsigned char>(data.a) });
                }
            }
        }

        // Bottom side of image.
        for (int x = 0; x < _width - x_resolution; x += x_resolution) {
            data = glm::vec4(0.0f);

            for (int j = _height - y_remainder; j < _height; ++j) {
                for (int i = 0; i < x_resolution; ++i) {
                    data += get_pixel(x + i, j);
                }
            }
            data /= (float)(x_resolution * y_remainder);

            // Fill pixel data to be the same color.
            for (int j = 0; j < y_remainder; ++j) {
                for (int i = 0; i < x_resolution; ++i) {
                    lower_res.set_pixel(x + i, _height - y_remainder + j, pixel_data { static_cast<unsigned char>(data.r), static_cast<unsigned char>(data.g), static_cast<unsigned char>(data.b), static_cast<unsigned char>(data.a) });
                }
            }
        }

        // Bottom right corner.
        data = glm::vec4(0.0f);

        for (int j = _height - y_remainder; j < _height; ++j) {
            for (int i = _width - x_remainder; i < _width; ++i) {
                data += get_pixel(i, j);
            }
        }
        data /= (float)(x_remainder * y_remainder);

        // Fill pixel data to be the same color.
        for (int j = 0; j < y_remainder; ++j) {
            for (int i = 0; i < x_remainder; ++i) {
                lower_res.set_pixel(_width - x_remainder + i, _height - y_remainder + j, pixel_data { static_cast<unsigned char>(data.r), static_cast<unsigned char>(data.g), static_cast<unsigned char>(data.b), static_cast<unsigned char>(data.a) });
            }
        }


        return lower_res;
    }

    void image::to_ascii() const {
    }


    image::pixel_data::pixel_data(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r),
                                                                                                        g(g),
                                                                                                        b(b),
                                                                                                        a(a) {
    }

    image::pixel_data::operator glm::vec4() const {
        return glm::vec4(r, g, b, a);
    }

}