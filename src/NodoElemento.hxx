#ifndef NODOELEMENTO_H
#define NODOELEMENTO_H

#include "CodigoElemento.hxx"
#include "NodoCodificacion.hxx"
#include <vector>

template <typename T> struct NodoElemento : public NodoCodificacion<T>
{
    T dato;

    NodoElemento<T>() = default;
    NodoElemento<T>(T elemento, freq_t frecuencia);

    std::vector<CodigoElemento<T>> codigos_elementos(
        std::string codigo) override;
};

template <typename T>
inline NodoElemento<T>::NodoElemento(T elemento, freq_t frecuencia)
    : NodoCodificacion<T>{frecuencia}, dato{elemento}
{
}

template <typename T>
inline std::vector<CodigoElemento<T>> NodoElemento<T>::codigos_elementos(
    std::string codigo)
{
    return std::vector<CodigoElemento<T>>{
        CodigoElemento<T>{this->dato, this->frecuencia, codigo}};
}

#endif // NODOELEMENTO_H
