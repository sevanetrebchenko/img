# img

This project is to experiment with various image processing algorithms, written in pure C++ with minimal dependencies. Note that code was written with simplicity in mind, and large abstractions or optimizations are hence avoided.

## Dependencies

- [GLM](https://github.com/g-truc/glm)
- [STB](https://github.com/nothings/stb) (`stb_image.h`, `stb_image_write.h`)

## Algorithms and Results

Additional image processing results (not shown below) can be viewed in `assets/generated/`.

| Converting images to grayscale |
| :---: |
| ![Grayscale](https://github.com/sevanetrebchenko/img/blob/master/assets/generated/desert/desert_grayscale.png) <br> Algorithm outputs the average color for the overall square for each pixel contained within it. |

- Converting images to grayscale
- Downscaling image resolution
- Converting images to ascii
- Recoloring images via K-Means Clustering algorithm
- Dithering algorithms
  - Error diffusion
  - Floyd-Steinberg
