#include "io/pgm_image.hpp"
#include <fstream>
#include <iostream>
#include <utility>

namespace core::io {

    ImagePGM load_pgm(const std::string& filepath) {

        //Abrimos el archivo en modo binario
        std::ifstream file(filepath, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Error: No se puedo abrir el archivo PGM " << filepath << "\n";
            return {};
        }

        //Leemos el numero magicp (debe ser "P5")
        std::string magic_number;
        file >> magic_number;

        if (magic_number != "P5") {
            std::cerr << "Error: El archivo no es un formato PGM binario (P5) valido\n";
            return {};
        }

        //Leemos ancho, alto y el valor maximo de gris (255)
        int width{0};
        int height{0};
        int max_val{0};

        file >> width >> height >> max_val;

        //Ignoramos el salto de linea '\n' restante despues del 255
        file.ignore(256, '\n');

        //Calculamos el total de bytes: Ancho * Alto (1 bytes por píxel)
        const std::size_t total_bytes = static_cast<std::size_t>(width) * height;
        std::vector<uint8_t> buffer(total_bytes);

        //Lectura masiva de los pixeles directo del SDD a la memoria RAM
        file.read(reinterpret_cast<char*>(buffer.data()), total_bytes);

        //Regresamos la estructura usando std::move para transferir la RAM 
        return ImagePGM{width, height, std::move(buffer)};
    }

    bool save_pgm(const std::string& filepath, const ImagePGM& img) {

        //Abrimos el archivo en modo binario 
        std::ofstream file(filepath, std::ios::binary);

        if(!file.is_open()) {
            std::cerr << "Error: No se puede crear/abrir el archivo de salida " << filepath << "\n";
            return false;
        }

        //Encabezado de PGM binario (P5)
        file << "P5\n" << img.width << " " << img.height << "\n255\n";

        //Volcado masivo de píxeles al disco 
        file.write(reinterpret_cast<const char*>(img.pixels.data()), img.pixels.size());

        return true;
    }
}