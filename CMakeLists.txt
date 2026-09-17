# 1. Indicamos la versión mínima de CMake que vamos a usar
cmake_minimum_required(VERSION 3.20)

# 2. Le damos un nombre al proyecto
project(ImageProcessingNEON LANGUAGES CXX)

# 3. Exigimos C++20 para aprovechar el estándar moderno
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 4. Le decimos a CMake dónde están las cabeceras (.hpp)
# Esto evita que tengas que mover archivos o usar rutas largas al incluir
include_directories(include)

# 5. Banderas de optimización para Clang ARM64
add_compile_options(-Wall -Wextra -O3 -mcpu=native)

# 6. Definimos explícitamente los archivos fuente de la librería/proyectos
set(PROJECT_SOURCES
   src/color/Gray_neon.cpp
   src/color/Binarization_neon.cpp
   )

# 7. Creamos el ejecutable uniendo el main y los módulos de procesamiento
add_executable(image_processor
   playground/main.cpp
   ${PROJECT_SOURCES}
   )   
