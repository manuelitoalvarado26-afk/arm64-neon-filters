#ifndef IFILTER_HPP
#define IFILTER_HPP

#include <cstddef>
#include <cstdint>

namespace core {

    class IFilter {
        public:
        virtual ~IFilter() = default;
        virtual void process(const uint8_t* src, uint8_t* dst, std::size_t total_pixels) = 0;
    };
}

#endif