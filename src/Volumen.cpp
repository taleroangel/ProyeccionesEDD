#include "Volumen.h"
#include "Imagen.h"

#include <cassert>
#include <iomanip>
#include <iostream>
#include <queue>
#include <sstream>
#include <stack>
#include <stdexcept>

Volumen::Volumen(std::string nombre_base, int tam)
    : nombre_base(nombre_base), tam_volumen(tam)
{
    if (tam > MAX_IMAGENES_VOLUMEN || tam < MIN_IMAGENES_VOLUMEN)
        throw std::exception();

    // Get max pixel
    int max_pixel = 0;

    int last_ancho = 0, last_alto = 0;

    // Por cada imágen que hay que cargar
    for (int i = 1; i <= tam; i++)
    {
        std::ostringstream temp;
        temp << std::setw(2) << std::setfill('0') << i;
        std::string numero_base = temp.str();
        this->volumen.push(Imagen(nombre_base + numero_base + EXTENSION_PGM));

        if (this->volumen.front().get_max_tam() > max_pixel)
            max_pixel = this->volumen.front().get_max_tam();

        if (i == 1)
        {
            last_alto = this->volumen.front().get_alto();
            last_ancho = this->volumen.front().get_ancho();
        }
        else // Si alguna imagen no tiene ancho y alto igual al anterior
             // ancho y alto
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

    this->max_val = max_pixel;

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

void Volumen::crear_proyeccion(std::string criterio, char direccion,
                               std::string nombre_archivo)
{
    // Aquí queda el bloque con todas las imágenes
    int alto, ancho;
    std::queue<Imagen> imagenes{};

    // Rotar el bloque para que queden en el eje de las X
    switch (direccion)
    {
    case 'x': // Se deja tal cual
        alto = this->alto;
        ancho = this->ancho;
        imagenes = this->get_volumen();
        break;

    case 'y':
        alto = this->tam_volumen;
        ancho = this->alto;

        // Por cada trozo vertical del volumen
        for (int k = 0; k < this->ancho; k++)
        {
            Imagen::matriz_t mtx = Imagen::matriz_vacia(ancho, alto);
            std::queue<Imagen> copia(this->get_volumen());
            // Por cada capa horizontal del volumen
            for (int c = 0; !copia.empty(); c++)
            {
                Imagen::matriz_t capa = copia.front().get_pixeles();
                // Por cada pixel en la capa
                for (int i = 0; i < this->alto; i++)
                {
                    mtx[c][i] = capa[i][k];
                }
                copia.pop();
            }

            // Meter la nueva capa
            imagenes.push(Imagen{mtx});
        }
        break;

    case 'z':
        alto = this->tam_volumen;
        ancho = this->ancho;

        // Por cada trozo vertical del volumen
        for (int k = 0; k < this->alto; k++)
        {
            Imagen::matriz_t mtx = Imagen::matriz_vacia(ancho, alto);
            std::queue<Imagen> copia(this->get_volumen());
            // Por cada capa horizontal del volumen
            for (int c = 0; !copia.empty(); c++)
            {
                Imagen::matriz_t capa = copia.front().get_pixeles();
                // Por cada pixel en la capa
                for (int i = 0; i < this->ancho; i++)
                {
                    mtx[c][i] = capa[k][i];
                }
                copia.pop();
            }

            // Meter la nueva capa
            imagenes.push(Imagen{mtx});
        }
        break;

    default:
        throw std::invalid_argument("Dirección errónea");
    }

    // Almacenar el resultado
    Imagen::matriz_t resultado = Imagen::matriz_vacia(ancho, alto);

    // Crear la proyección de la imágen
    if (criterio.compare("promedio") == 0)
    {
        // Buscar en cada imágen
        while (!imagenes.empty())
        {
            Imagen::matriz_t pixeles = imagenes.front().get_pixeles();

            // Sumar cada imagen al total
            for (int i = 0; i < alto; i++)
                for (int j = 0; j < ancho; j++)
                    resultado[i][j] += pixeles[i][j];

            imagenes.pop();
        }

        // Sacar el promedio total
        for (int i = 0; i < alto; i++)
            for (int j = 0; j < ancho; j++)
                resultado[i][j] /= this->tam_volumen;
    }
    else if (criterio.compare("maximo") == 0)
    {
        // Buscar en cada imágen el máximo
        while (!imagenes.empty())
        {
            Imagen::matriz_t pixeles = imagenes.front().get_pixeles();

            // Buscar el máximo de cada imágen
            for (int i = 0; i < alto; i++)
                for (int j = 0; j < ancho; j++)
                    if (pixeles[i][j] > resultado[i][j])
                        resultado[i][j] = pixeles[i][j];

            imagenes.pop();
        }
    }

    else if (criterio.compare("minimo") == 0)
    {
        Imagen::llenar_matrix(resultado, this->max_val + 1);
        // Buscar en cada imágen el máximo
        while (!imagenes.empty())
        {
            Imagen::matriz_t pixeles = imagenes.front().get_pixeles();

            // Buscar el máximo de cada imágen
            for (int i = 0; i < alto; i++)
                for (int j = 0; j < ancho; j++)
                    if (pixeles[i][j] < resultado[i][j])
                        resultado[i][j] = pixeles[i][j];

            imagenes.pop();
        }
    }

    // TODO crear los criterios
    else
    {
        throw std::invalid_argument("Dirección errónea");
    }

    // Grabar el resultado y girarlo adecuadamente
    (direccion != 'x' ? (Imagen::reflejo_vertical(Imagen{resultado}))
                      : Imagen{resultado})
        .guardar_archivo(nombre_archivo);
}