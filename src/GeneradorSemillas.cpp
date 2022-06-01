#include "GeneradorSemillas.h"

#include <cstddef>
#include <cstdlib>
#include <iostream>

#include "Arista.hxx"
#include "Imagen.h"

GeneradorSemillas::~GeneradorSemillas() {
    delete this->grafo;
    this->grafo = nullptr;
}

GeneradorSemillas::GeneradorSemillas(Imagen imagen) {
    // Obtener los pixeles
    Imagen::matriz_t matrix = imagen.get_pixeles();

    /*
    * > Generar el Grafo

    1. Tomar cada píxel y crear dos aristas, una con el nodo vecino derecho
    y otro con el nodo vecino de abajo

        - Para cada nodo calcular su vecino derecho e inferior

        Al ser NO_DIRIGIO creariamos una malla de nodos de conexiones
        desde cualquier nodo a cualquiera de sus vecinos

    */

    std::vector<Arista<Imagen::pixel_t>> aristas{};

    // Por cada fila
    for (size_t i = 0; i < matrix.size(); i++) {
        // Por cada columna
        for (size_t j = 0; j < matrix[i].size(); j++) {
            // Derecha
            if ((j + 1) < matrix[i].size())
                aristas.push_back({matrix[i][j], matrix[i][j + 1],
                                   static_cast<double>(
                                       abs(matrix[i][j + 1] - matrix[i][j]))});

            // Abajo
            if ((i + 1) < matrix.size())
                aristas.push_back({matrix[i][j], matrix[i + 1][j],
                                   static_cast<double>(
                                       abs(matrix[i + 1][j] - matrix[i][j]))});
        }
    }

    // Crear el grafo
    this->grafo = new Grafo<Imagen::pixel_t>{aristas};

    std::cout << grafo << std::endl;
}
