
#include <iostream>

#include "img/image.h"
#include "img/processor.h"

int main(int argc, char* argv[]) {

    img::image image("assets/images/journey.jpg");
//
//    image.process().to_grayscale().save();
//
//    image.process().to_lower_resolution(20, 20).save();
//
//    for (int k = 2; k <= 16; ++k) {
//        image.process().k_means(k).save();
//    }
//
//    image.process().dither_error_diffusion().save();
//    image.process().dither_floyd_steinberg().save();
//    image.process().dither_false_floyd_steinberg().save();
//    image.process().dither_jarvis_judice_ninke().save();
//    image.process().dither_stucki().save();
//    image.process().dither_atkinson().save();
//    image.process().dither_burkes().save();
//    image.process().dither_sierra().save();
//    image.process().dither_two_row_sierra().save();
//    image.process().dither_sierra_lite().save();
//    image.process().dither_bayer(2, 2).save();
//    image.process().dither_bayer(4, 4).save();
//    image.process().dither_bayer(8, 8).save();
//    image.process().dither_bayer(4, 6).save();

    image.process().voronoi(50).save();
    image.process().voronoi(100).save();
    image.process().voronoi(200).save();
    image.process().voronoi(500).save();
    image.process().voronoi(1000).save();
    image.process().voronoi(2000).save();
    image.process().voronoi(5000).save();

    return 0;
}