#ifndef CORE_IO_PGM_IMAGE_HPP
#define CORE_IO_PGM_IMAGE_HPP

#include <string.h>
#include <vector>
#include <cstdint>

namespace core::io {

  struct ImagePGM {
    int width{0};
    int height{0};
    std::vector<uint8_t> pixels;
};

ImagePGM load_pgm(const std::string& filepath);

bool save_pgm(const std::string& filepath, const ImagePGM& img);
}

#endif