#ifndef _PROYECCION_H_
#define _PROYECCION_H_

#include "Imagen.h"
#include "Volumen.h"

#include <string>

class Proyeccion
{
private:
	std::string criterio;
	char direccion; //* Sólo puede ser 'x', 'y' o 'z'
	std::string nombre_archivo;

public:
	Proyeccion() = delete;
	Proyeccion(std::string criterio, char direccion, std::string nombre_archivo);

	//* Crear la proyección 2D
	void crearProyeccion(Volumen volumen);

	std::string obtener_criterio() const;
	void fijar_criterio(std::string criterio);

	char obtener_direccion() const;
	void fijar_direccion(char direccion);

	std::string obtener_nombre_archivo() const;
	void fijar_nombre_archivo(std::string nombre_archivo);
};

#endif //_PROYECCION_H_