#ifndef __CONTROLADOR_HXX__
#define __CONTROLADOR_HXX__

#include "Console.hxx"
#include "Imagen.hxx"
#include "Volumen.hxx"

#include <vector>
#include <memory>

//! Esta clase es estática, todo dentro de ella debe ser estática
class Controlador
{
private:
	// Apuntador inteligente a una imagen
	static Imagen *imagen_cargada;
	static Volumen *volumen_cargado;

public:
	static void limpiar();

	//* Componente 1

	static void cargar_imagen(Command::arguments_t args);

	static void cargar_volumen(Command::arguments_t args);

	static void info_imagen(Command::arguments_t args);

	static void info_volumen(Command::arguments_t args);

	static void proyeccion_2d(Command::arguments_t args);

	//* Componente 2

	static void codificar_imagen(Command::arguments_t args);

	static void decodificar_archivo(Command::arguments_t args);

	static void segmentar(Command::arguments_t args);
};

#endif // __CONTROLADOR_HXX__