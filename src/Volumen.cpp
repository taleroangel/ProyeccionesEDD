#include "Volumen.h"

#include <sstream>
#include <iomanip>
#include <stdexcept>

Volumen::Volumen(std::string nombre_base, int tam)
	: nombre_base(nombre_base), tam_volumen(tam)
{
	if (tam > MAX_IMAGENES_VOLUMEN || tam < MIN_IMAGENES_VOLUMEN)
		throw std::exception();

	// Por cada imágen que hay que cargar
	for (int i = 1; i <= tam; i++)
	{
		std::ostringstream temp;
		temp << std::setw(2) << std::setfill('0') << i;
		std::string numero_base = temp.str();

		this->volumen.push(Imagen(nombre_base + numero_base + EXTENSION_PGM));
	}

	// Ancho y alto son iguales al ancho y alto de las imágenes
	this->ancho = this->volumen.front().obtener_ancho();
	this->alto = this->volumen.front().obtener_alto();
}

std::string Volumen::to_string() const
{
	return "(proceso satisfactorio) Volumen cargado en memoria: " + nombre_base +
		   ", tamaño: " + std::to_string(tam_volumen) +
		   ", ancho: " + std::to_string(ancho) +
		   ",  alto: " + std::to_string(alto);
}

int Volumen::obtener_ancho() const
{
	return ancho;
}
void Volumen::fijar_ancho(int ancho)
{
	this->ancho = ancho;
}

int Volumen::obtener_alto() const
{
	return alto;
}

void Volumen::fijar_alto(int alto)
{
	this->alto = alto;
}

std::string Volumen::obtener_nombre_base() const { return nombre_base; }
void Volumen::fijar_nombre_base(std::string nombreBase) { nombre_base = nombreBase; }

int Volumen::obtener_tam_volumen() const { return tam_volumen; }
void Volumen::fijar_tam_volumen(int tamVolumen) { tam_volumen = tamVolumen; }

std::queue<Imagen> Volumen::obtener_volumen() const { return volumen; }
void Volumen::fijar_volumen(std::queue<Imagen> volumen_) { volumen = volumen_; }