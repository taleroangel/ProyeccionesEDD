#ifndef NODOCODIFICACION_H
#define NODOCODIFICACION_H

#include "CodigoElemento.hxx"
#include <string>
#include <vector>

template <typename T> struct NodoCodificacion
{
    freq_t frecuencia = 0;

    NodoCodificacion() = default;
    NodoCodificacion(freq_t frecuencia);

    virtual ~NodoCodificacion();

    virtual std::vector<CodigoElemento<T>> codigos_elementos(
        std::string codigo) = 0;

    bool operator<(const NodoCodificacion<T> &other) const;
    bool operator>(const NodoCodificacion<T> &other) const;
    bool operator<=(const NodoCodificacion<T> &other) const;
    bool operator>=(const NodoCodificacion<T> &other) const;
};

template <typename T>
inline NodoCodificacion<T>::NodoCodificacion(freq_t frecuencia)
    : frecuencia(frecuencia)
{
}

template <typename T> inline NodoCodificacion<T>::~NodoCodificacion()
{
}

template <typename T>
inline bool NodoCodificacion<T>::operator<(
    const NodoCodificacion<T> &other) const
{
    return this->frecuencia < other.frecuencia;
}

template <typename T>
inline bool NodoCodificacion<T>::operator>(
    const NodoCodificacion<T> &other) const
{
    return other < *this;
}

template <typename T>
inline bool NodoCodificacion<T>::operator<=(
    const NodoCodificacion<T> &other) const
{
    return !(other < *this);
}

template <typename T>
inline bool NodoCodificacion<T>::operator>=(
    const NodoCodificacion<T> &other) const
{
    return !(*this < other);
}

#endif // NODOCODIFICACION_H
