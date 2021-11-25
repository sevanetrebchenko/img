
#include "img/image.h"
#include "img/utility.h"
#include "img/processor.h"

#define FULL_QUALITY 100

namespace img {

    image::image(const std::string &filepath) : file(filepath),
                                                width(-1),
                                                height(-1),
                                                channels(-1),
                                                total(-1),
                                                data(nullptr),
                                                stb_allocated(true) {
//        stbi_set_flip_vertically_on_load(true);

        // Load in texture data.
        data = stbi_load(file.path.c_str(), &width, &height, &channels, 0);
        if (!data) {
            throw std::runtime_error("Failed to open file: " + file.path);
        }

        // Print image information.
        std::cout << "Asset: " << file.name + '.' + file.extension << std::endl;
        std::cout << "Width: " << width << std::endl;
        std::cout << "Height: " << height << std::endl;
        std::cout << "Channels: " << channels << ' ';
        std::cout << (channels >= 4 ? "[RGBA]" : "[RGB]") << std::endl;

        total = width * height * channels;
    }

    image::image(const std::string &filepath, int width, int height, int channels) : file(filepath),
                                                                                     width(width),
                                                                                     height(height),
                                                                                     channels(channels),
                                                                                     total(width * height * channels),
                                                                                     data(nullptr),
                                                                                     stb_allocated(false) {
        data = new unsigned char[width * height * channels];
    }

    image::~image() {
        stb_allocated ? stbi_image_free(data) : delete[] data;
    }

    image::image(const image& other) : file(other.file),
                                       width(other.width),
                                       height(other.height),
                                       total(other.total),
                                       channels(other.channels),
                                       data(nullptr),
                                       stb_allocated(false) // Deep copy happened, don't free with stb.
                                       {
        // Deep copy data.
        data = new unsigned char[width * height * channels];
        memcpy(data, other.data, width * height * channels);
    }

    image& image::operator=(const image& other) {
        if (&other == this) {
            return *this;
        }

        file = other.file;
        width = other.width;
        height = other.height;
        channels = other.channels;
        total = other.total;
        stb_allocated = false; // Deep copy happened, don't free with stb.

        // Deep copy data.
        data = new unsigned char[width * height * channels];
        memcpy(data, other.data, width * height * channels);

        return *this;
    }

    pixel image::get_pixel(int x, int y) const {
        int offset = (x + width * y) * channels;
        assert(offset < total); // Validate image offset.
        unsigned char *address = data + offset;

        return {address[0], address[1], address[2], channels >= 4 ? address[3] : static_cast<unsigned char>(0xff)};
    }

    void image::set_pixel(int x, int y, const pixel &value) {
        int offset = (x + width * y) * channels;
        assert(offset < total); // Validate image offset.
        unsigned char *address = data + offset;

        address[0] = value.r;
        address[1] = value.g;
        address[2] = value.b;

        // Only set alpha if there is an alpha channel.
        if (channels >= 4) {
            address[3] = value.a;
        }
    }

    void image::set_pixel(int x, int y, const glm::vec4 &value) {
        int offset = (x + width * y) * channels;
        assert(offset < total); // Validate image offset.
        unsigned char *address = data + offset;

        address[0] = static_cast<unsigned char>(value.r);
        address[1] = static_cast<unsigned char>(value.g);
        address[2] = static_cast<unsigned char>(value.b);

        // Only set alpha if there is an alpha channel.
        if (channels >= 4) {
            address[3] = static_cast<unsigned char>(value.a);
        }
    }

    void image::save() const {
        const std::string &ext = file.extension;

        // Create directory for file if it doesn't exist.
        bool directory_exists = std::filesystem::exists(file.directory);
        if (!directory_exists || (directory_exists && std::filesystem::is_directory(file.directory))) {
            std::filesystem::create_directory(file.directory);
        }

        if (ext == "jpg" || ext == "JPG" || ext == "jpeg" || ext == "JPEG") {
            stbi_write_jpg(file.path.c_str(), width, height, channels, data, FULL_QUALITY);
        }
        else if (ext == "png" || ext == "PNG") {
            int stride = width * channels;
            stbi_write_png(file.path.c_str(), width, height, channels, data, stride);
        }
        else {
            std::cerr << "Invalid extension." << std::endl;
        }
    }

    processor image::process() const {
        return img::processor(*this);
    }

    int image::get_height() const {
        return height;
    }

    int image::get_width() const {
        return width;
    }

    int image::get_channels() const {
        return channels;
    }

}