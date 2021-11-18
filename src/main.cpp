
#include <iostream>

#include "img/image.h"
#include "img/process.h"

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
    img::process(image).to_grayscale().to_lower_resolution(20, 20).get().save();

    return 0;
}