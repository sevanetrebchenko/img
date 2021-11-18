
#include "img/pixel.h"

namespace img {

    pixel::pixel(unsigned char r,
                 unsigned char g,
                 unsigned char b,
                 unsigned char a) : r(r),
                                    g(g),
                                    b(b),
                                    a(a)
                                    {
    }

    pixel::pixel(const glm::vec4 &value) : r(static_cast<unsigned char>(value.r)),
                                           g(static_cast<unsigned char>(value.g)),
                                           b(static_cast<unsigned char>(value.b)),
                                           a(static_cast<unsigned char>(value.a))
                                           {
    }

    pixel::~pixel() = default;
    pixel::pixel(const pixel &other) = default;

    pixel &pixel::operator=(const pixel &other) {
        if (&other == this) {
            return *this;
        }

        r = other.r;
        g = other.g;
        b = other.b;
        a = other.a;

        return *this;
    }

    pixel::operator glm::vec3() const {
        return { r, g, b };
    }

    pixel::operator glm::vec4() const {
        return { r, g, b, a };
    }

}
