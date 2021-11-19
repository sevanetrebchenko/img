
#include <iostream>

#include "img/image.h"
#include "img/processor.h"

void usage() {
    std::cout << "img [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "-h | --help        print help" << std::endl;
}

int main(int argc, char* argv[]) {
//    if (argc == 1) {
//        usage();
//    }
//
//    // Print out instructions on how to use.
//    if (argc < 2) {
//        std::cerr << "error in calling program" << std::endl;
//        return 1;
//    }

    img::image image("assets/images/the_weeknd.jpg");

    for (int k = 2; k <= 16; ++k) {
        std::cout << "processing: " << k << std::endl;
        image.process().k_means(k, true).save();
    }

    return 0;
}