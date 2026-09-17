#ifndef GRAYSCALE_STATIC_HPP
#define GRAYSCALE_STATIC_HPP

#include <cstdint>
#include <cstddef>

namespace core {

    class GrayscalerStatic {

        public:  
        static void process (const uint8_t* src, uint8_t* dst, std::size_t total_pixels);
    };
    
}

#endif