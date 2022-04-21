#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "ArbolCodificacion.hxx"
#include "Imagen.h"
#include <cstddef>
#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

class Huffman
{
  public:
    using byte = uint8_t;
    using word = uint16_t;
    using lword = uint64_t;

  public:
    word ancho = 0;
    word alto = 0;
    byte maximo = 255;

    const Imagen &imagen;
    ArbolCodificacion<byte> *arbol = nullptr;
    std::unordered_map<int, CodigoElemento<byte>> codigos;

  public:
    /**
     * @brief Construye la codificación Huffman de una imágen
     * @param img Imagen
     */
    Huffman(const Imagen &img);

    ~Huffman();

    /**
     * @brief Guardar la imágen de Huffman en un archivo
     *
     * @param nombre_archivo
     */
    void guardar_archivo(std::string nombre_archivo);
};

#endif // HUFFMAN_H
