#ifndef CODIGOELEMENTO_HXX
#define CODIGOELEMENTO_HXX

#include <ostream>
#include <sstream>
#include <string>

template <typename T> struct CodigoElemento
{
    T elemento;
    unsigned long frecuencia;
    std::string codigo;

    CodigoElemento() = default;
    CodigoElemento(const CodigoElemento<T> &) = default;
    CodigoElemento &operator=(const CodigoElemento<T> &) = default;
    CodigoElemento(T elemento, unsigned long frecuencia, std::string codigo);

    std::string to_string();
};

template <typename T>
inline CodigoElemento<T>::CodigoElemento(T elemento, unsigned long frecuencia,
                                         std::string codigo)
    : elemento{elemento}, frecuencia{frecuencia}, codigo{codigo}
{
}

template <typename T> inline std::string CodigoElemento<T>::to_string()
{
    std::stringstream string;
    string << "[Elemento: " << this->elemento << ", Freq: " << frecuencia
           << ", Codigo: " << codigo << ']';
    return string.str();
}

#endif // CODIGOELEMENTO_HXX
