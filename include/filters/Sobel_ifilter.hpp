#ifndef SOBEL_IFILTER_HPP
#define SOBEL_IFILTER_HPP

#include "IFilter.hpp"
#include "static/Sobel_static.hpp"

namespace core {

    class SobelIFilter : public IFilter {
        private:
        int m_width;
        int m_height;

        public:
        explicit SobelIFilter(int width, int height)
        : m_width(width), m_height(height) {}

        void process(const uint8_t* src, uint8_t* dst, std::size_t total_pixels) override {
            (void)total_pixels;

            core::SobelStatic::process(src, dst, m_width, m_height);
        }
    };
}

#endif