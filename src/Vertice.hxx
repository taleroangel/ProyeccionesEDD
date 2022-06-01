#ifndef VERTICE_HXX
#define VERTICE_HXX

#include <ostream>

/* --------- Declaraciones --------- */

template <typename T>
class Vertice {
    /* --------- Constructores --------- */
   public:
    Vertice() = delete;
    Vertice(T);
    Vertice(Vertice &&) = default;
    Vertice(const Vertice &) = default;
    Vertice &operator=(Vertice &&) = default;
    Vertice &operator=(const Vertice &) = default;
    ~Vertice() = default;

    /* --------- Operadores --------- */
    bool operator<(const Vertice<T> &other) const;
    bool operator>(const Vertice<T> &other) const;
    bool operator<=(const Vertice<T> &other) const;
    bool operator>=(const Vertice<T> &other) const;

    bool operator==(const Vertice<T> &other) const;
    bool operator!=(const Vertice<T> &other) const;

    template <typename K>
    friend std::ostream &operator<<(std::ostream &os, const Vertice<K> &rhs);

    /* --------- Atributos --------- */
   protected:
    T value;

    /* --------- Métodos --------- */
   public:
    T get_value() const;
};

/* --------- Definiciones --------- */

template <typename T>
Vertice<T>::Vertice(T dato) : value(dato) {}

template <typename T>
inline bool Vertice<T>::operator<(const Vertice<T> &other) const {
    return this->value < other.value;
}

template <typename T>
inline bool Vertice<T>::operator>(const Vertice<T> &other) const {
    return other < *this;
}

template <typename T>
inline bool Vertice<T>::operator<=(const Vertice<T> &other) const {
    return !(other < *this);
}

template <typename T>
inline bool Vertice<T>::operator>=(const Vertice<T> &other) const {
    return !(*this < other);
}

template <typename T>
inline bool Vertice<T>::operator==(const Vertice<T> &other) const {
    return this->value == other.value;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const Vertice<T> &rhs) {
    os << rhs.value;
    return os;
}

template <typename T>
inline bool Vertice<T>::operator!=(const Vertice<T> &other) const {
    return !(*this == other);
}

template <typename T>
inline T Vertice<T>::get_value() const {
    return this->value;
}

#endif  // VERTICE_HXX
