#ifndef _IMAGEN_H_
#define _IMAGEN_H_

#include <string>
#include <vector>
#include <cstdlib>

class Imagen
{
public:
	// Fila de una matriz
	using elemento_t = int;
	using fila_t = std::vector<elemento_t>;
	// Matriz de vectores
	//* El alto es el vector de afuera
	//* El ancho son los vectores de adentro (fila)
	using matriz_t = std::vector<fila_t>;

private:
	std::string formato_imagen;
	int ancho;
	int alto;
	int max_tam;
	matriz_t matriz_pixeles;
	std::string nombre_archivo;

public:
	Imagen() = default;
	Imagen(std::string formato, matriz_t matriz_pixeles);
	Imagen(std::string nombre_archivo);
	bool guardarArchivo(std::string nombre_archivo);

	// Getters y setters
	std::string obtener_formato() const;
	void fijar_formato(const std::string &formatoImagen);

	int obtener_ancho() const;
	void fijar_ancho(int ancho_);

	int obtener_alto() const;
	void fijar_alto(int alto_);

	int obtener_max_tam() const;
	void fijar_max_tam(int maxTam);

	matriz_t obtener_pixeles() const;
	void fijar_pixeles(matriz_t matrizPixeles);

	std::string obtener_nombre_archivo() const;
	void fijar_nombre_archivo(std::string nombre);

	std::string to_string();
};

#endif //_IMAGEN_H_