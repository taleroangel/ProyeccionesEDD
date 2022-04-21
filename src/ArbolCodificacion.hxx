#ifndef ARBOLCODIFICACION_HXX
#define ARBOLCODIFICACION_HXX

#include "CodigoElemento.hxx"
#include "NodoCodificacion.hxx"
#include "NodoElemento.hxx"
#include "NodoFrecuencia.hxx"
#include <iostream>
#include <map>
#include <queue>
#include <string>
#include <vector>

template <typename T> class ArbolCodificacion
{
  private:
    NodoCodificacion<T> *raiz = nullptr;

  public:
    ArbolCodificacion() = delete;
    ArbolCodificacion(std::map<T, int> frecuencias);

    ~ArbolCodificacion();

    std::vector<CodigoElemento<T>> codigos_elementos();

    static bool comparador(const NodoCodificacion<T> *a,
                           const NodoCodificacion<T> *b)
    {
        return a->frecuencia < b->frecuencia;
    };
};

template <typename T>
inline ArbolCodificacion<T>::ArbolCodificacion(std::map<T, int> frecuencias)
{
    // Tabla con las frecuencias
    std::priority_queue<NodoCodificacion<T> *> cola{};

    for (std::pair<T, int> x : frecuencias)
        cola.push(new NodoElemento<T>{x.first, x.second});

    // Insertar en el árbol
    int n = cola.size();
    for (int i = 0; i < n - 1; i++)
    {
        NodoFrecuencia<T> *nodoZ = new NodoFrecuencia<T>{};

        nodoZ->hijoIzq = cola.top();
        cola.pop();
        nodoZ->hijoDer = cola.top();
        cola.pop();

        nodoZ->frecuencia =
            nodoZ->hijoIzq->frecuencia + nodoZ->hijoDer->frecuencia;

        // Guardar el nodo
        cola.push(nodoZ);
    }

    this->raiz = cola.top();
    cola.pop();
}

template <typename T> inline ArbolCodificacion<T>::~ArbolCodificacion()
{
    if (this->raiz != nullptr)
    {
        delete raiz;
        raiz = nullptr;
    }
}

template <typename T>
inline std::vector<CodigoElemento<T>> ArbolCodificacion<T>::codigos_elementos()
{
    return this->raiz->codigos_elementos("");
}

#endif // ARBOLCODIFICACION_HXX
