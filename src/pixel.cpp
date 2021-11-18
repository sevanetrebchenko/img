
#include "pixel.h"

namespace img {

    pixel::pixel(unsigned char r,
                 unsigned char g,
                 unsigned char b,
                 unsigned char a) : r(r),
                                    g(g),
                                    b(b),
                                    a(a) {
    }

    pixel::~pixel() = default;

    pixel::operator glm::vec4() const {
        return { r, g, b, a };
    }

}
