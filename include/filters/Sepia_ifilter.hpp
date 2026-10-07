#ifndef SEPIA_IFILTER_HPP
#define SEPIA_IFILTER_HPP

#include "IFilter.hpp"
#include "static/Sepia_static.hpp"

namespace core {

    class SepiaIFilter : public IFilter {
        public:
         SepiaIFilter() = default;

        void process(const uint8_t* src, uint8_t* dst, std::size_t total_pixels) override {
            core::SepiaStatic::process(src, dst, total_pixels);
        }
    };
}

#endif 