#ifndef GENERADORSEMILLAS_H
#define GENERADORSEMILLAS_H

#include "Grafo.hxx"
#include "Imagen.h"
class GeneradorSemillas {
    /* --------- Atributos privados --------- */
   private:
    Grafo<Imagen::pixel_t, GRAFO_NO_DIRIGIDO> *grafo = nullptr;

   public:
    /* --------- Constructores por defecto y copias --------- */
    GeneradorSemillas() = delete;  // No se permite constructor sin parámetros
    GeneradorSemillas(GeneradorSemillas &&) = default;
    GeneradorSemillas(const GeneradorSemillas &) = default;
    GeneradorSemillas &operator=(GeneradorSemillas &&) = default;
    GeneradorSemillas &operator=(const GeneradorSemillas &) = default;
    ~GeneradorSemillas();

    /* --------- Constructor a partir de una imagen --------- */
    GeneradorSemillas(Imagen imagen);
};

#endif  // GENERADORSEMILLAS_H
