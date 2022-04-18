#include "Volumen.h"

#include <cassert>
#include <iomanip>
#include <queue>
#include <sstream>
#include <stdexcept>

std::queue<Imagen> Volumen::rotar_bloque(const char direccion, int &ancho,
                                         int &alto)
{
    // Bloque a retornar
    std::queue<Imagen> bloque;

    // Calcular el bloque
    switch (direccion)
    {
    case 'x': // Retorna el bloque tal cual
        ancho = this->ancho;
        alto = this->alto;
        return std::queue<Imagen>(this->volumen);

    case 'y':
        alto = this->tam_volumen;
        ancho = this->alto;

        // Obtener una imágen por cada capa
        for (int k = 0; k < this->ancho; k++)
        {
            // Crear una copia del bloque
            std::queue<Imagen> copia(this->volumen);

            Imagen::matriz_t newm = Imagen::matriz_vacia(alto, ancho);

            for (int c = 0; !copia.empty(); c++)
            {
                Imagen old = copia.front();
                // Copiar la capa al vector de bloques
                for (int i = 0; i < old.get_alto(); i++)
                    newm[c][i] = old.get_pixeles()[i][k];

                copia.pop();
            }

            // Guardar la imágen
            bloque.push(Imagen{newm});
        }

        // Retornar el blque
        return bloque;

    default: // La dirección no es válida
        throw std::invalid_argument("No es una dirección válida");
    }
}

Volumen::Volumen(std::string nombre_base, int tam)
    : nombre_base(nombre_base), tam_volumen(tam)
{
    if (tam > MAX_IMAGENES_VOLUMEN || tam < MIN_IMAGENES_VOLUMEN)
        throw std::exception();

    int last_ancho = 0, last_alto = 0;

    // Por cada imágen que hay que cargar
    for (int i = 1; i <= tam; i++)
    {
        std::ostringstream temp;
        temp << std::setw(2) << std::setfill('0') << i;
        std::string numero_base = temp.str();
        this->volumen.push(Imagen(nombre_base + numero_base + EXTENSION_PGM));

        if (i == 1)
        {
            last_alto = this->volumen.front().get_alto();
            last_ancho = this->volumen.front().get_ancho();
        }
        else // Si alguna imagen no tiene ancho y alto igual al anterior ancho
             // y alto
        {
            if ((last_ancho != this->volumen.front().get_ancho()) ||
                (last_alto != this->volumen.front().get_alto()))
            {
                throw std::exception();
            }
        }
    }

    // Ancho y alto son iguales al ancho y alto de las imágenes
    this->ancho = this->volumen.front().get_ancho();
    this->alto = this->volumen.front().get_alto();

    // El tamaño debe ser igual
    assert(static_cast<int>(this->volumen.size()) == tam_volumen);
}

std::string Volumen::to_string() const
{
    return "(proceso satisfactorio) Volumen cargado en memoria: " +
           nombre_base + ", tamaño: " + std::to_string(tam_volumen) +
           ", ancho: " + std::to_string(ancho) +
           ",  alto: " + std::to_string(alto);
}

int Volumen::get_ancho() const
{
    return ancho;
}

void Volumen::set_ancho(int ancho)
{
    this->ancho = ancho;
}

int Volumen::get_alto() const
{
    return alto;
}

void Volumen::set_alto(int alto)
{
    this->alto = alto;
}

std::string Volumen::get_nombre_base() const
{
    return nombre_base;
}
void Volumen::set_nombre_base(std::string nombreBase)
{
    nombre_base = nombreBase;
}

int Volumen::get_tam_volumen() const
{
    return tam_volumen;
}

void Volumen::set_tam_volumen(int tamVolumen)
{
    tam_volumen = tamVolumen;
}

std::queue<Imagen> Volumen::get_volumen() const
{
    return volumen;
}

void Volumen::set_volumen(std::queue<Imagen> volumen_)
{
    volumen = volumen_;
}

// TODO crear la proyección
void Volumen::crear_proyeccion(std::string criterio, char direccion,
                               std::string nombre_archivo)
{
    int ancho, alto;
    std::queue<Imagen> bloque = this->rotar_bloque(direccion, ancho, alto);
    int tam = bloque.size();
    Imagen::matriz_t resultado = Imagen::matriz_vacia(alto, ancho);

    // PROMEDIO
    while (!bloque.empty())
    {
        Imagen imagen = bloque.front();

        // Sumar al resultado
        for (int i = 0; i < alto; i++)
            for (int j = 0; j < ancho; j++)
                resultado[i][j] += imagen.get_pixeles()[i][j];

        bloque.pop();
    }

    for (int i = 0; i < alto; i++)
        for (int j = 0; j < ancho; j++)
            resultado[i][j] /= tam;

    Imagen img{resultado};
    img.guardar_archivo(nombre_archivo);
}