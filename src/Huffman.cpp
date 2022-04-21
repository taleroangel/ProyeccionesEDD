#include "Huffman.h"

#include <cstring>
#include <exception>
#include <fstream>
#include <map>
#include <queue>
#include <stdexcept>
#include <utility>

Huffman::Huffman(const Imagen &img)
    : ancho(img.get_ancho()), alto(img.get_alto()), maximo(255), imagen(img)
{
    if (img.get_max_tam() > 255)
        throw std::invalid_argument("La imagen no está normalizada");

    // Mapa de frecuencias
    std::map<byte, int> frecuencias;
    for (int i = 0; i <= 255; i++)
        frecuencias[(byte)i] = 0;

    // Calcular frecuencias en la imágen
    Imagen::matriz_t pixeles = img.get_pixeles();
    for (int i = 0; i < img.get_alto(); i++)
        for (int j = 0; j < img.get_ancho(); j++)
            frecuencias[(
                byte)pixeles[i][j]]++; // Cada vez que se encuentra un
                                       // pixel, le suma 1 a su frecuencia

    // Se crea el árbol de codificacion
    this->arbol = new ArbolCodificacion<byte>{frecuencias};

    // Se obtienen las frecuencias y valores de los bytes
    std::vector<CodigoElemento<byte>> codigos =
        this->arbol->codigos_elementos();

    // Codigos en blanco
    for (int i = 0; i <= 255; i++)
        this->codigos.insert(
            std::make_pair((byte)i, CodigoElemento<byte>{(byte)i, 0, ""}));

    /// Insertar codigos del árbol
    for (CodigoElemento<byte> codigo : codigos)
        this->codigos[codigo.elemento] = codigo;
}

Huffman::~Huffman()
{
    if (this->arbol != nullptr)
    {
        delete arbol;
        arbol = nullptr;
    }
}

void Huffman::guardar_archivo(std::string nombre_archivo)
{
    // Abrir el archivo
    std::ofstream archivo(nombre_archivo, std::ios::binary | std::ios::out);

#ifdef _DEBUG_
    std::ofstream debugf(nombre_archivo + ".debug.txt", std::ios::out);
#endif

    // Verificar que se pudo abrir
    if (!archivo.is_open())
    {
        throw std::runtime_error("No se pudo abrir el archivo");
    }

    // Escribir las dimensiones y el máximo
    archivo.write((char *)(&this->ancho), sizeof(word));
    archivo.write((char *)(&this->alto), sizeof(word));
    archivo.write((char *)(&this->maximo), sizeof(byte));

#ifdef _DEBUG_
    debugf << "W: " << (int)this->ancho << '\t' << "WORD (2)" << '\n';
    debugf << "H: " << (int)this->alto << '\t' << "WORD (2)" << '\n';
    debugf << "MAX: " << (int)this->maximo << '\t' << "BYTE (1)" << '\n';
#endif

    // Escribir todas las frecuencias
    for (int i = 0; i <= 255; i++)
    {
        archivo.write((char *)&this->codigos[i].frecuencia, sizeof(lword));

#ifdef _DEBUG_
        debugf << (int)i << '\t' << this->codigos[i].frecuencia << '\t'
               << this->codigos[i].codigo << '\t' << "LWORD (8)" << '\n';
#endif
    }

    // Cerrar el archivo
    archivo.close();

#ifdef _DEBUG_
    debugf.close();
#endif
}
