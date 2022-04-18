#ifndef __CONTROLADOR_H__
#define __CONTROLADOR_H__

#include "Consola.h"
#include "Imagen.h"
#include "Volumen.h"

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

	static void cargar_imagen(Comando::arguments_t args);

	static void cargar_volumen(Comando::arguments_t args);

	static void info_imagen(Comando::arguments_t args);

	static void info_volumen(Comando::arguments_t args);

	static void proyeccion_2d(Comando::arguments_t args);

	//* Componente 2

	static void codificar_imagen(Comando::arguments_t args);

	static void decodificar_archivo(Comando::arguments_t args);

	static void segmentar(Comando::arguments_t args);
};

#endif // __CONTROLADOR_H__