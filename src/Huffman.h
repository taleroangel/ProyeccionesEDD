#ifndef HUFFMAN_H
#define HUFFMAN_H

#include "ArbolCodificacion.hxx"
#include "Imagen.h"
#include <cstddef>
#include <cstdint>
#include <map>
#include <queue>
#include <string>

class Huffman
{
  public:
    using byte_t = uint8_t;
    using word_t = uint16_t;
    using dword_t = uint32_t;
    using lword_t = uint64_t;

  public:
    word_t ancho = 0;
    word_t alto = 0;
    byte_t maximo = 255;

    const Imagen &imagen;
    ArbolCodificacion<byte_t> *arbol = nullptr;
    std::map<byte_t, CodigoElemento<byte_t>> codigos;

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
