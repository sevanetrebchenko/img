
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <algorithm>

void ConvertToNativeSeparators(char* path) {
#ifdef _WIN32
    const char separator = '/';
    const char native = '\\';
#else
    const char separator = '\\';
    const char native = '/';
#endif

    for (std::size_t character = 0; character < strlen(path); ++character) {
        char* location = path + character;

        if (*location == separator) {
            *location = native;
        }
    }
}

void PrintUsage() {
    std::cout << "itoa [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-h | --help        print help" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        PrintUsage();
    }

    // Print out instructions on how to use.
    if (argc < 2) {
        std::cerr << "error in calling program" << std::endl;
        return 1;
    }

    char* filename = argv[1]; // First argument is program name.
    ConvertToNativeSeparators(filename);

    stbi_set_flip_vertically_on_load(true);

    // Load in texture data.
    int width, height, channels;
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (!data) {
        std::cerr << "failed to open provided path" << std::endl;
        return 1;
    }

    std::cout << filename << std::endl;

    stbi_image_free(data);
    return 0;
}