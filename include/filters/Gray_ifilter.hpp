#ifndef GRAY_IFILTER_HPP
#define GRAY_IFILTER_HPP

#include "IFilter.hpp"
#include "static/Gray_static.hpp"

namespace core {

    class GrayIFilter : public IFilter {
        public:
        GrayIFilter() = default;

        void process(const uint8_t* src, uint8_t* dst, std::size_t total_pixels) override {
            GrayscalerStatic::process(src, dst, total_pixels);
        }
    }; 
}

#endif