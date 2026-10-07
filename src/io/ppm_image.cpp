#include "io/ppm_image.hpp"
#include <fstream>
#include <iostream>
#include <utility>

namespace core::io {

    ImagePPM load_ppm(const std::string& filepath) {

        //Abrimos el archivo de entrada en modo binario 
        std::ifstream file(filepath, std::ios::binary);

        if (!file.is_open()) {
            std::cerr << "Eror: No se pudo abrir el archivo" << filepath << "\n";
            return {};
        }

        //Leemos el numero magico (debe ser "P6")
        std::string magic_number;
        file >> magic_number;

        if (magic_number != "P6") {
            std::cerr << "Error: El archivo no es un formato PPM binario (P6) valido\n";
            return {};
        }

        //Leemos ancho, alto y el valor maximo de color (255)
        int width{0};
        int height{0};
        int max_val{0};

        file >> width >> height >> max_val;

        //Ignoramos el salto de linea '\n' restante despues del 255
        file.ignore(256, '\n');

        //Calculamosn el total de bytes: Ancho * Alto * 3 canales (RGB)
        const std::size_t total_bytes = static_cast<std::size_t>(width) * height * 3;
        std::vector<uint8_t> buffer(total_bytes);

        //Lecturs masiva de los pixeles directo del SDD a la memoria RAM
        file.read(reinterpret_cast<char*>(buffer.data()), total_bytes);

        //Regresamos la estructura usando std::move para transferir la RAM
        return ImagePPM{width, height, std::move(buffer)};
    }

        bool save_ppm(const std::string& filepath, const ImagePPM& img) {

        //Abrimos el archivo de salida en modo binario
        std::ofstream file(filepath, std::ios::binary);   

        if (!file.is_open()) {
        std::cerr << "Error: No se pudo crear/abrir el archivo de salida" << filepath << "\n";
        return false;
    }
        //Escribimos el encabezado P6 en formato de texto 
        file << "P6\n" << img.width << " " << img.height << "\n255\n";

        //Volcado masivo de los pixeles procesados desde la RAM al disco 
        file.write(reinterpret_cast<const char*>(img.pixels.data()), img.pixels.size());

        return file.good();
    }
}