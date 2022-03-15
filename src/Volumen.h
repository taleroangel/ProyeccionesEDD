#ifndef _VOLUMEN_H_
#define _VOLUMEN_H_

#include "Imagen.h"
#include <string>
#include <queue>

#define MAX_IMAGENES_VOLUMEN 99
#define MIN_IMAGENES_VOLUMEN 2
#define EXTENSION_PGM ".pgm"

class Volumen
{
private:
	std::string nombre_base;
	int tam_volumen;
	int ancho;
	int alto;
	std::queue<Imagen> volumen;

public:
	Volumen() = default;
	Volumen(std::string nombre_base, int tam);

	std::string to_string() const;

	std::string obtener_nombre_base() const;
	void fijar_nombre_base(std::string nombreBase);

	int obtener_tam_volumen() const;
	void fijar_tam_volumen(int tamVolumen);

	int obtener_ancho() const;
	void fijar_ancho(int ancho);

	int obtener_alto() const;
	void fijar_alto(int alto);

	std::queue<Imagen> obtener_volumen() const;
	void fijar_volumen(std::queue<Imagen> volumen_);
};

#endif //_VOLUMEN_H_