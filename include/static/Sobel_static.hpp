#ifndef SOBEL_STATIC_HPP
#define SOBEL_STATIC_HPP

#include <cstdint>

namespace core {

    class SobelStatic {

        public:
        static void process(const uint8_t* src, uint8_t* dst, int width, int height);
    };
}

#endif