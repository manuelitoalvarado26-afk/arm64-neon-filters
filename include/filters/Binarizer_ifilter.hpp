#ifndef BINARIZER_FILTER_HHP
#define BINARIZER_FILTER_HHP

#include "IFilter.hpp"
#include "static/Binarizer_static.hpp"

namespace core {
    
    class BinarizerIFilter : public IFilter  {
    private:
    uint8_t m_threshold;

    public:
    explicit BinarizerIFilter(uint8_t threshold = 128)
    : m_threshold(threshold) {}

    void process(const uint8_t* src, uint8_t* dst, std::size_t total_pixels) override {
        core::BinarizeStatic::process(src, dst, total_pixels, m_threshold);
    }
  }; 
}



#endif