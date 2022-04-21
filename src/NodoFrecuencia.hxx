#ifndef NODOFRECUENCIA_H
#define NODOFRECUENCIA_H

#include "NodoCodificacion.hxx"

template <typename T> struct NodoFrecuencia : public NodoCodificacion<T>
{
    NodoCodificacion<T> *hijoIzq = nullptr;
    NodoCodificacion<T> *hijoDer = nullptr;

    NodoFrecuencia() = default;
    NodoFrecuencia(int frecuencia);
    ~NodoFrecuencia();

    std::vector<CodigoElemento<T>> codigos_elementos(
        std::string codigo) override;
};

template <typename T>
inline NodoFrecuencia<T>::NodoFrecuencia(int frecuencia)
    : NodoCodificacion<T>{frecuencia}
{
}

template <typename T> inline NodoFrecuencia<T>::~NodoFrecuencia()
{
    if (this->hijoIzq != nullptr)
    {
        delete hijoIzq;
        hijoIzq = nullptr;
    }

    if (this->hijoDer != nullptr)
    {
        delete hijoDer;
        hijoDer = nullptr;
    }
}

template <typename T>
inline std::vector<CodigoElemento<T>> NodoFrecuencia<T>::codigos_elementos(
    std::string codigo)
{
    std::vector<CodigoElemento<T>> izquierdo{};
    if (this->hijoIzq != nullptr)
        izquierdo = this->hijoIzq->codigos_elementos("0" + codigo);

    std::vector<CodigoElemento<T>> derecho{};
    if (this->hijoDer != nullptr)
        derecho = this->hijoDer->codigos_elementos("1" + codigo);

    std::vector<CodigoElemento<T>> resultado{};
    resultado.insert(resultado.end(), izquierdo.begin(), izquierdo.end());
    resultado.insert(resultado.end(), derecho.begin(), derecho.end());

    return resultado;
}

#endif // NODOFRECUENCIA_H
