
#include "pch.h"
#include "image.h"

void usage() {
    std::cout << "img [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-h | --help        print help" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        usage();
    }

    // Print out instructions on how to use.
    if (argc < 2) {
        std::cerr << "error in calling program" << std::endl;
        return 1;
    }

    img::image image(argv[1]);
    image.to_grayscale().save();
    image.to_ascii(2);

    return 0 ;
}