#ifndef BINARIZARE_STATIC_HHP
#define BINARIZARE_STATIC_HHP

#include <cstdint>
#include <cstddef>

namespace core {

    class BinarizeStatic {

        public:
        static void process (const uint8_t* src, uint8_t* dst, std::size_t total_pixels, uint8_t treashold);
    };

    #endif
}