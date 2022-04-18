#include "Controlador.h"

#include <cstring>
#include <iostream>

//* Inicializar variables estáticas
Imagen *Controlador::imagen_cargada = nullptr;
Volumen *Controlador::volumen_cargado = nullptr;

//! Componente 1

void Controlador::cargar_imagen(Comando::arguments_t args)
{
	if (args.size() != 1)
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);

	// Cargar la imagen en memoria
	try
	{
		// Si ya hay un volume cargado
		if (imagen_cargada != nullptr)
		{
			std::cerr << "(mensaje de error) ya hay una imagen cargada\n";
			return;
		}

		// Crear la imagen
		imagen_cargada = new Imagen(args[0]);
	}
	catch (std::exception &e)
	{
		std::cerr << "(mensaje de error) La imagen " + args[0] + " no ha podido ser cargada\n";
		imagen_cargada = nullptr;
		return;
	}

	std::cout << "(proceso satisfactorio) La imagen '" + args[0] + "' ha sido cargada" << std::endl;
}

void Controlador::limpiar()
{
	if (imagen_cargada != nullptr)
	{
		delete imagen_cargada;
		imagen_cargada = nullptr;
	}

	if (volumen_cargado != nullptr)
	{
		delete volumen_cargado;
		volumen_cargado = nullptr;
	}
}

void Controlador::cargar_volumen(Comando::arguments_t args)
{
	if (args.size() != 2)
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);

	// Cargar volumen
	try
	{
		// Si ya hay un volumen cargado
		if (volumen_cargado != nullptr)
		{
			std::cerr << "(mensaje de error) ya hay un volumen cargado\n";
			return;
		}

		volumen_cargado = new Volumen(args[0], std::stoi(args[1]));
	}

	catch (std::exception &e)
	{
		std::cerr << "(mensaje de error) El volumen " + args[0] + " no ha podido ser cargado\n";
		volumen_cargado = nullptr;
		return;
	}

	std::cout << "(proceso satisfactorio) El volumen " + args[0] + " ha sido cargado" << std::endl;
}

void Controlador::info_imagen(Comando::arguments_t args)
{
	if (!args.empty())
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);

	if (imagen_cargada == nullptr)
	{
		std::cerr << "(mensaje de error) No hay imagen cargada en memoria\n";
		return;
	}

	std::cout << imagen_cargada->to_string() << std::endl;
}

void Controlador::info_volumen(Comando::arguments_t args)
{
	if (!args.empty())
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);

	if (volumen_cargado == nullptr)
	{
		std::cerr << "(mensaje de error) No hay volumen cargado en memoria\n";
		return;
	}

	std::cout << volumen_cargado->to_string() << std::endl;
}

void Controlador::proyeccion_2d(Comando::arguments_t args)
{
	if (args.size() != 3)
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);

	//* Validar la dirección
	if (args[0].size() != 1)
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);
	switch (args[0].c_str()[0]) // Criterio
	{
		// Casos válidos
	case 'x':
	case 'y':
	case 'z':
		break;

	default:
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);
		break;
	}

	//* Validar el criterio
	if (args[1] != "minimo" &&
		args[1] != "maximo" &&
		args[1] != "promedio")
	{
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);
	}

	//* Crear la proyección
	try
	{
		// Si el volumen aún no se carga
		if (volumen_cargado == nullptr)
			throw std::exception();

		// Crear la proyeccion
		volumen_cargado->crear_proyeccion(
			args[1],
			args[0][0],
			args[2]);
	}
	catch (std::exception &e)
	{
		std::cerr << "(mensajes de error)\n"
				  << "El volumen aún no ha sido cargado en memoria\n"
				  << "La proyección 2D del volumen en memoria no ha podido ser generada\n";
		return;
	}

	std::cout << "(proceso satisfactorio) La proyección 2D del volumen en memoria ha sido generada" << std::endl;
}

//! Componente 2

void Controlador::codificar_imagen(Comando::arguments_t args)
{
	if (args.size() != 1)
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);
}

void Controlador::decodificar_archivo(Comando::arguments_t args)
{
	if (args.size() != 2)
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);
}

void Controlador::segmentar(Comando::arguments_t args)
{
	if (args.size() <= 1)
		throw Comando::Error(Comando::Error::Type::INVALID_ARGS);
}
