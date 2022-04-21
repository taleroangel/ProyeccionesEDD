#include "Huffman.h"
#include "CodigoElemento.hxx"

#include <cstring>
#include <exception>
#include <fstream>
#include <map>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <utility>

Huffman::Huffman(const Imagen &img)
    : ancho(img.get_ancho()), alto(img.get_alto()), maximo(255), imagen(img)
{
    // Verificar normalización
    if (img.get_max_tam() > 255)
        throw std::invalid_argument("La imagen no está normalizada");

    // Construir mapa de frecuencias
    std::map<byte_t, freq_t> frecuencias{};
    for (int i = 0; i <= 255; i++)
        frecuencias[i] = 0;

    // Calcular frecuencias en la imágen
    Imagen::matriz_t pixeles = img.get_pixeles();
    for (int i = 0; i < img.get_alto(); i++)
        for (int j = 0; j < img.get_ancho(); j++)
            frecuencias[(byte_t)pixeles[i][j]]++;
    // Cada vez que se encuentra un
    // pixel, le suma 1 a su frecuencia

    // Retirar todos los valores en 0
    std::map<byte_t, freq_t> freq_arbol{};
    for (std::pair<byte_t, freq_t> frecuencia : frecuencias)
        if (frecuencia.second != 0)
            freq_arbol.insert(frecuencia);

    // Se crea el árbol de codificacion
    this->arbol = new ArbolCodificacion<byte_t>{freq_arbol};

    // Se obtienen las frecuencias y valores de los bytes
    for (CodigoElemento<byte_t> elemento : this->arbol->codigos_elementos())
        this->codigos.insert({elemento.elemento, elemento});
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
    //* Abrir el archivo
    std::ofstream archivo(nombre_archivo, std::ios::binary | std::ios::out);
#ifdef _DEBUG_
    std::ofstream debugf(nombre_archivo + ".debug.txt", std::ios::out);
#endif

    // Verificar que se pudo abrir
    if (!archivo.is_open())
        throw std::runtime_error("No se pudo abrir el archivo");

    //* Header
    lword_t size = static_cast<lword_t>(this->codigos.size());
    archivo.write((char *)(&this->ancho), sizeof(word_t));
    archivo.write((char *)(&this->alto), sizeof(word_t));
    archivo.write((char *)(&this->maximo), sizeof(byte_t));
    archivo.write((char *)(&size), sizeof(lword_t));

#ifdef _DEBUG_
    debugf << "# HEADER #\n";
    debugf << "WIDTH\t" << (int)this->ancho << '\t' << "WORD (2)" << '\n';
    debugf << "HEIGHT\t" << (int)this->alto << '\t' << "WORD (2)" << '\n';
    debugf << "VALMAX\t" << (int)this->maximo << '\t' << "BYTE (1)" << '\n';
    debugf << "NDATA\t" << (int)size << '\t' << "LWORD (8)" << '\n';
    debugf << "# FREQ TABLE #\n";
#endif

    //* Frecuencias
    // Escribir todas las frecuencias
    for (byte_t i = 0; i < this->codigos.size(); i++)
    {
        archivo.write((char *)&codigos[i].elemento, sizeof(byte_t));
        archivo.write((char *)&codigos[i].frecuencia, sizeof(dword_t));

#ifdef _DEBUG_
        debugf << static_cast<int>(codigos[i].elemento) << "\tBYTE (1)\t"
               << codigos[i].frecuencia << "\t\tDWORD (4)\t" << codigos[i].codigo
               << "\t\t\t\t BITS(" << codigos[i].codigo.size() << ")\n";
#endif
    }

    //* Calcular codigo general
    std::stringstream generalss;
    Imagen::matriz_t pixels = imagen.get_pixeles();

    // Tomar todos los elementos de la imagen y colocarlos en codigo
    for (int i = 0; i < imagen.get_alto(); i++)
        for (int j = 0; j < imagen.get_ancho(); j++)
            for (byte_t k = 0; k < this->codigos.size(); k++)
                if (static_cast<Imagen::elemento_t>(codigos[k].elemento) ==
                    pixels[i][j])
                    generalss << codigos[k].codigo;

    const std::string general = generalss.str();

#ifdef _DEBUG_
    debugf << "# CODE #\n" << general << "\n# END #";
#endif

    //* Transformar a bytes
    for (int i = 0; i < general.size();)
    {
        byte_t byte = 0x0000;
        // Transform every bit
        for (int k = 8; k > 0; k--, i++)
        {
            if (i >= general.size())
                break;
            if (general.at(i) == '1')
                (byte |= (1 << (k - 1)));
        }

        // Escribir el bit
        archivo.write((char *)&byte, sizeof(byte_t));
    }

    //* Cerrar archivos
    archivo.close();
#ifdef _DEBUG_
    debugf.close();
#endif
}
