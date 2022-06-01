#include "GeneradorSemillas.h"

#include <cstddef>
#include <cstdlib>
#include <limits>
#include <utility>

#include "Arista.hxx"
#include "Imagen.h"
#include "Semilla.h"

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

    std::vector<Arista<Semilla>> aristas{};

    // Por cada fila
    for (size_t i = 0; i < matrix.size(); i++) {
        // Por cada columna
        for (size_t j = 0; j < matrix[i].size(); j++) {
            // Derecha
            if ((j + 1) < matrix[i].size())
                aristas.push_back({Semilla{i, j, matrix[i][j]},
                                   Semilla{i, (j + 1), matrix[i][j + 1]},
                                   static_cast<double>(
                                       abs(matrix[i][j + 1] - matrix[i][j]))});

            // Abajo
            if ((i + 1) < matrix.size()) {
                aristas.push_back({Semilla{i, j, matrix[i][j]},
                                   Semilla{(i + 1), j, matrix[i + 1][j]},
                                   static_cast<double>(
                                       abs(matrix[i + 1][j] - matrix[i][j]))});
            }
        }
    }

    // Crear el grafo
    this->grafo = new Grafo<Semilla>{aristas};
}

std::vector<std::pair<Semilla, Grafo<Semilla>::dijkstra_path>>
GeneradorSemillas::generar_caminos(std::vector<Semilla> semillas) {
    // Resultado
    std::vector<std::pair<Semilla, Grafo<Semilla>::dijkstra_path>> caminos{};

    // Por cada semilla
    for (size_t i = 0; i < semillas.size(); i++) {
        // Generar el Dijkstra y Guardarlo
        caminos.push_back(std::make_pair(
            semillas[i], this->grafo->dijkstra_algorithm(semillas[i])));
    }

    return caminos;
}
std::vector<std::vector<Semilla>> GeneradorSemillas::generar_matriz_etiquetas(
    Imagen imagen,
    std::vector<std::pair<Semilla, Grafo<Semilla>::dijkstra_path>> caminos) {
    // Inicializar matrices
    Imagen::matriz_t matrix = imagen.get_pixeles();

    // Inicializar matriz de resultados
    std::vector<std::vector<Semilla>> resultado{};
    // Agregar las filas
    for (size_t i = 0; i < matrix.size(); i++)
        resultado.push_back(std::vector<Semilla>{});

    // Por cada fila
    for (size_t i = 0; i < matrix.size(); i++) {
        // Por cada columna
        for (size_t j = 0; j < matrix[i].size(); j++) {
            // Generar la semilla inicial
            Semilla pixel{i, j, matrix[i][j], 0};

            double min_costo = std::numeric_limits<double>::infinity();
            int min_etiqueta = std::numeric_limits<double>::max();

            // Por cada camino en el arreglo
            for (size_t c = 0; c < caminos.size(); c++) {
                double peso = Grafo<Semilla, GRAFO_NO_DIRIGIDO>::weight_of(
                    caminos[c].second, pixel);

                // Hallar la etiqueta de menor costo
                if (peso < min_costo) {
                    min_costo = peso;
                    min_etiqueta = caminos[c].first.etiqueta;
                }
            }

            // Asignar la etiqueta de menor costo
            pixel.etiqueta = min_etiqueta;

            // Meter en la matriz la semilla
            resultado[i].push_back(pixel);
        }
    }

    return resultado;
}
