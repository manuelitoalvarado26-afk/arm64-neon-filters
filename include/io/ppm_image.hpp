#ifndef CORE_IO_PPM_IMAGE_HPP
#define CORE_IO_PPM_IMAGE_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace core::io {

  struct ImagePPM {
    int width{0};
    int height{0};
    std::vector<uint8_t> pixels;
};

ImagePPM load_ppm(const std::string& filepath);

bool save_ppm(const std::string& filepath, const ImagePPM& img);
}

#endif