#ifndef BINARIZARE_FILTER_HHP
#define BINARIZARE_FILTER_HHP
#include "IFilter.hpp"

namespace core {

    class BinarizeFilter : public IFilter {
        private:
        uint8_t threshold_;

        public:
        explicit BinarizeFilter (uint8_t threshold_);
        void process (const uint8_t* src, uint8_t* dst, std::size_t total_pixels) override;
    };

    #endif
}