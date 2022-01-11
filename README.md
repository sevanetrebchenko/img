# img

This project is to experiment with various image processing algorithms, written in pure C++ with minimal dependencies. Note that code was written with simplicity in mind, and large abstractions or optimizations are hence avoided.

## Dependencies

- [GLM](https://github.com/g-truc/glm)
- [STB](https://github.com/nothings/stb) (`stb_image.h`, `stb_image_write.h`)

## Algorithms and Results

Additional image processing results, as well as the original images used for the GIFs, can be viewed in `assets/generated/`. Original images can be viewed in `assets/images`. 
  
| Original image |
| :---: |
| ![Original](https://github.com/sevanetrebchenko/img/blob/master/assets/images/toucan.jpg) |

| Converting images to grayscale |
| :---: |
| ![Grayscale](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_grayscale.jpg) |

| Downscaling image resolution |
| :---: |
| ![Downscale](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_20x20px.jpg) |

| Recoloring images via the [K-Means Clustering algorithm](https://en.wikipedia.org/wiki/K-means_clustering) |
| :---: |
| ![K-Means](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_k_means.gif) <br> The above GIF shows the original image processed with k-values ranging from 2 to 16. |

| Various [dithering](https://en.wikipedia.org/wiki/Dither) algorithms |
| :---: |
| ![Error Diffusion](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_error_diffusion.jpg) <br> Error Diffusion <br><br> ![Floyd Steinberg](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_floyd_steinberg.jpg) <br> Floyd-Steinberg <br><br> ![False Floyd Steinberg](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_false_floyd_steinberg.jpg) <br> False Floyd-Steinberg <br><br> ![False Floyd Steinberg](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_jarvis_judice_ninke.jpg) <br> Jarvis-Judice-Ninke <br><br> ![Stucki](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_stucki.jpg) <br> Stucki <br><br> ![Atkinson](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_atkinson.jpg) <br> Atkinson <br><br> ![Burkes](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_burkes.jpg) <br> Burkes <br><br> ![Sierra](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_sierra.jpg) <br> Sierra <br><br> ![Sierra-Lite](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_sierra_lite.jpg) <br> Sierra-Lite <br><br> ![Two Row Sierra](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_two_row_sierra.jpg) <br> Two-Row Sierra <br><br> ![Bayer2x2](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_bayer_2x2.jpg) <br> 2x2 Bayer Matrix (ordered dithering) <br><br> ![Bayer4x4](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_bayer_4x4.jpg) <br> 4x4 Bayer Matrix (ordered dithering) <br><br> ![Bayer8x8](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_dither_bayer_8x8.jpg) <br> 8x8 Bayer Matrix (ordered dithering) |

| [Voronoi Diagram](https://en.wikipedia.org/wiki/Voronoi_diagram) |
| :---: |
| ![Voronoi](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/toucan/toucan_voronoi.gif) <br> The above GIF cycles between 50, 100, 200, 500, 1,000, 2,000, 5,000, and 10,000 Voronoi regions. |
 
