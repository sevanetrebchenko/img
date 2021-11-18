
#ifndef IMG_PIXEL_H
#define IMG_PIXEL_H

#include "img.h"

namespace img {

    struct pixel {
        pixel(unsigned char r = 0x00,
              unsigned char g = 0x00,
              unsigned char b = 0x00,
              unsigned char a = 0xff);
        explicit pixel(const glm::vec4& value);

        ~pixel();

        pixel(const pixel &other);
        pixel &operator=(const pixel &other);

        // Convert pixel data to other type.
        explicit operator glm::vec3() const;
        explicit operator glm::vec4() const;

        unsigned char r;
        unsigned char g;
        unsigned char b;
        unsigned char a;
    };

}

#endif //IMG_PIXEL_H
