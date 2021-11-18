
#ifndef IMG_PIXEL_H
#define IMG_PIXEL_H

namespace img {

    class pixel {
        public:
            explicit pixel(unsigned char r = 0x00, unsigned char g = 0x00, unsigned char b = 0x00, unsigned char a = 0xff);
            ~pixel();

            // Convert pixel data to glm::vec4.
            explicit operator glm::vec4() const;

        private:
            unsigned char r;
            unsigned char g;
            unsigned char b;
            unsigned char a;
    };

}

#endif //IMG_PIXEL_H
