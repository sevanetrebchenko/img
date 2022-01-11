# img

This project is to experiment with various image processing algorithms, written in pure C++ with minimal dependencies. Note that code was written with simplicity in mind, and large abstractions or optimizations are hence avoided.

## Dependencies

- [GLM](https://github.com/g-truc/glm)
- [STB](https://github.com/nothings/stb) (`stb_image.h`, `stb_image_write.h`)

## Algorithms and Results

Additional image processing results, as well as the original images used for the GIFs, can be viewed in `assets/generated/`. Original images can be viewed in `assets/images`. Note that, due to the size of the example image used below, some visual details may be lost. Please refer to the aforementioned image processing result directory for the highest visual quality image.

| Original image |
| :---: |
| ![Original](https://github.com/sevanetrebchenko/img/blob/master/assets/images/journey.png) |

| Converting images to grayscale |
| :---: |
| ![Grayscale](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_grayscale.png) <br> Algorithm converts image to grayscale, with color conversion constants taken [from this Wikipedia article](https://en.wikipedia.org/wiki/Grayscale). |

| Downscaling image resolution |
| :---: |
| ![Downscale](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_20x20px.png) <br> Algorithm outputs the average color for the overall square for each pixel contained within it. The above image has been downscaled from 2560 x 1080 pixels to 128 x 54 pixels (each square is 20 x 20 pixels in size). |

| Recoloring images via the [K-Means Clustering algorithm](https://en.wikipedia.org/wiki/K-means_clustering) |
| :---: |
| ![K-Means](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_k_means.gif) <br> Algorithm recolors the output image based on the k-means (number of individual colors, k). The higher the k-value, the more colors are present in the output image. The above GIF shows the original image processed with k-values ranging from 2 to 16. |

| Various [dithering](https://en.wikipedia.org/wiki/Dither) algorithms |
| :---: |
| ![Error Diffusion](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_error_diffusion.png) <br> Error Diffusion <br><br> ![Floyd Steinberg](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_floyd_steinberg.png) <br> Floyd-Steinberg <br><br> ![False Floyd Steinberg](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_false_floyd_steinberg.png) <br> False Floyd-Steinberg <br><br> ![False Floyd Steinberg](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_jarvis_judice_ninke.png) <br> Jarvis-Judice-Ninke <br><br> ![Stucki](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_stucki.png) <br> Stucki <br><br> ![Atkinson](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_atkinson.png) <br> Atkinson <br><br> ![Burkes](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_burkes.png) <br> Burkes <br><br> ![Sierra](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_sierra.png) <br> Sierra <br><br> ![Sierra-Lite](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_sierra_lite.png) <br> Sierra-Lite <br><br> ![Two Row Sierra](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_two_row_sierra.png) <br> Two-Row Sierra <br><br> ![Bayer2x2](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_bayer_2x2.png) <br> 2 x 2 Bayer Matrix (ordered dithering) <br><br> ![Bayer4x4](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_bayer_4x4.png) <br> 4 x 4 Bayer Matrix (ordered dithering) <br><br> ![Bayer8x8](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_bayer_8x8.png) <br> 8 x 8 Bayer Matrix (ordered dithering) <br><br> ![Bayer4x6](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_dither_bayer_4x6.png) <br> 4 x 6 Bayer Matrix (ordered dithering) <br><br> |

| [Voronoi Diagram](https://en.wikipedia.org/wiki/Voronoi_diagram) |
| :---: |
| ![Voronoi](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/journey/journey_voronoi.gif) <br> Algorithm generates uniformly distributed Vonoroi regions and outputs the average color for the region for each pixel contained within it. The above GIF shows the original image processed with 50, 100, 200, 500, 1,000, 2,000, 5,000, and 10,000 Voronoi regions. |
