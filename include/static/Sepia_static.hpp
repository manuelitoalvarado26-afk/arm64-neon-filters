#ifndef SEPIA_STATIC_HPP
#define SEPIA_STATIC_HPP

#include <cstdint>
#include <cstddef>

namespace core {

    class SepiaStatic {

        public:
        static void process(const uint8_t* src, uint8_t* dst, std::size_t total_pixels);
    };
}

#endif