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

	std::string get_nombre_base() const;
	void set_nombre_base(std::string nombreBase);

	int get_tam_volumen() const;
	void set_tam_volumen(int tamVolumen);

	int get_ancho() const;
	void set_ancho(int ancho);

	int get_alto() const;
	void set_alto(int alto);

	std::queue<Imagen> get_volumen() const;
	void set_volumen(std::queue<Imagen> volumen_);
};

#endif //_VOLUMEN_H_