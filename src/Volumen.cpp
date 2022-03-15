#include "Volumen.h"

#include <sstream>
#include <iomanip>
#include <stdexcept>

Volumen::Volumen(std::string nombre_base, int tam)
	: nombre_base(nombre_base), tam_volumen(tam)
{
	if (tam > MAX_IMAGENES_VOLUMEN || tam < MIN_IMAGENES_VOLUMEN)
		throw std::exception();

	int last_ancho = 0, last_alto = 0;

	// Por cada imágen que hay que cargar
	for (int i = 1; i <= tam; i++)
	{
		std::ostringstream temp;
		temp << std::setw(2) << std::setfill('0') << i;
		std::string numero_base = temp.str();

		this->volumen.push(Imagen(nombre_base + numero_base + EXTENSION_PGM));

		if (i == 1)
		{
			last_alto = this->volumen.front().get_alto();
			last_ancho = this->volumen.front().get_ancho();
		}
		else // Si alguna imagen no tiene ancho y alto igual al anterior ancho y alto
		{
			if ((last_ancho != this->volumen.front().get_ancho()) || (last_alto != this->volumen.front().get_alto()))
				throw std::exception();
		}
	}

	// Ancho y alto son iguales al ancho y alto de las imágenes
	this->ancho = this->volumen.front().get_ancho();
	this->alto = this->volumen.front().get_alto();
}

std::string Volumen::to_string() const
{
	return "(proceso satisfactorio) Volumen cargado en memoria: " + nombre_base +
		   ", tamaño: " + std::to_string(tam_volumen) +
		   ", ancho: " + std::to_string(ancho) +
		   ",  alto: " + std::to_string(alto);
}

int Volumen::get_ancho() const
{
	return ancho;
}
void Volumen::set_ancho(int ancho)
{
	this->ancho = ancho;
}

int Volumen::get_alto() const
{
	return alto;
}

void Volumen::set_alto(int alto)
{
	this->alto = alto;
}

std::string Volumen::get_nombre_base() const { return nombre_base; }
void Volumen::set_nombre_base(std::string nombreBase) { nombre_base = nombreBase; }

int Volumen::get_tam_volumen() const { return tam_volumen; }
void Volumen::set_tam_volumen(int tamVolumen) { tam_volumen = tamVolumen; }

std::queue<Imagen> Volumen::get_volumen() const { return volumen; }
void Volumen::set_volumen(std::queue<Imagen> volumen_) { volumen = volumen_; }

void Volumen::crear_proyeccion(
	std::string criterio,
	char direccion,
	std::string nombre_archivo)
{
	// Copiar las propiedades de una imagen
	Imagen copiar = this->get_volumen().front();
	Imagen proyeccion_imagen{}; // Donde se guarda la proyección

	proyeccion_imagen.set_formato(copiar.get_formato());
	proyeccion_imagen.set_max_tam(copiar.get_max_tam());
	proyeccion_imagen.set_nombre_archivo(nombre_archivo);

	//* De arriba para abajo
	if (direccion == 'x')
	{
		proyeccion_imagen.set_alto(copiar.get_alto());
		proyeccion_imagen.set_ancho(copiar.get_ancho());

		if (criterio == "promedio")
		{
			// Crear una matriz
			Imagen::matriz_t promedio;
			for (int i = 0; i < copiar.get_alto(); i++)
			{
				promedio.push_back(Imagen::fila_t());
				for (int j = 0; j < copiar.get_ancho(); j++)
				{
					promedio[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada fila
				for (int i = 0; i < pixeles.size(); i++)
				{
					// Por cada elemento
					for (int j = 0; j < pixeles[i].size(); j++)
					{
						promedio[i][j] += pixeles[i][j];
					}
				}

				imagenes.pop();
			}

			// Calcular el promedio
			for (int i = 0; i < promedio.size(); i++)
			{
				for (int j = 0; j < promedio[i].size(); j++)
				{
					promedio[i][j] = promedio[i][j] / copiar.get_alto();
				}
			}

			proyeccion_imagen.set_pixeles(promedio);
		}

		// Maximo
		else if (criterio == "maximo")
		{
			// Crear una matriz
			Imagen::matriz_t maximo;
			for (int i = 0; i < copiar.get_alto(); i++)
			{
				maximo.push_back(Imagen::fila_t());
				for (int j = 0; j < copiar.get_ancho(); j++)
				{
					maximo[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada fila
				for (int i = 0; i < pixeles.size(); i++)
				{
					// Por cada elemento
					for (int j = 0; j < pixeles[i].size(); j++)
					{
						if (pixeles[i][j] > maximo[i][j])
						{
							maximo[i][j] = pixeles[i][j];
						}
					}
				}

				imagenes.pop();
			}

			proyeccion_imagen.set_pixeles(maximo);
		}

		// Mínimo
		else if (criterio == "minimo")
		{
			// Crear una matriz
			Imagen::matriz_t minimo;
			for (int i = 0; i < copiar.get_alto(); i++)
			{
				minimo.push_back(Imagen::fila_t());
				for (int j = 0; j < copiar.get_ancho(); j++)
				{
					minimo[i].push_back(copiar.get_max_tam());
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada fila
				for (int i = 0; i < pixeles.size(); i++)
				{
					// Por cada elemento
					for (int j = 0; j < pixeles[i].size(); j++)
					{
						if (pixeles[i][j] < minimo[i][j])
						{
							minimo[i][j] = pixeles[i][j];
						}
					}
				}

				imagenes.pop();
			}

			proyeccion_imagen.set_pixeles(minimo);
		}
	}

	//* Desde el lado, de derecha a izquierda
	else if (direccion == 'y')
	{
		proyeccion_imagen.set_alto(this->get_volumen().size());
		proyeccion_imagen.set_ancho(this->get_alto());

		if (criterio == "promedio")
		{
			// Crear una matriz
			Imagen::matriz_t promedio;
			for (int i = 0; i < this->get_volumen().size(); i++)
			{
				promedio.push_back(Imagen::fila_t());
				for (int j = 0; j < this->get_alto(); j++)
				{
					promedio[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada fila
				for (int i = 0; i < pixeles.size(); i++)
				{
					Imagen::elemento_t acumulador = 0;
					// Por cada elemento
					for (int j = 0; j < pixeles[i].size(); j++)
					{
						acumulador += pixeles[i][j];
					}

					acumulador = acumulador / this->get_ancho();

					promedio[imagen][i] = acumulador;
				}

				imagenes.pop();
			}

			proyeccion_imagen.set_pixeles(promedio);
		}

		// Maximo
		else if (criterio == "maximo")
		{
			// Crear una matriz
			Imagen::matriz_t maximo;
			for (int i = 0; i < this->get_volumen().size(); i++)
			{
				maximo.push_back(Imagen::fila_t());
				for (int j = 0; j < this->get_alto(); j++)
				{
					maximo[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada fila
				for (int i = 0; i < pixeles.size(); i++)
				{
					Imagen::elemento_t max_val = 0;
					// Por cada elemento
					for (int j = 0; j < pixeles[i].size(); j++)
					{
						if (pixeles[i][j] > max_val)
						{
							max_val = pixeles[i][j];
						}
					}

					maximo[imagen][i] = max_val;
				}

				imagenes.pop();
			}

			proyeccion_imagen.set_pixeles(maximo);
		}

		// Mínimo
		else if (criterio == "minimo")
		{
			// Crear una matriz
			Imagen::matriz_t maximo;
			for (int i = 0; i < this->get_volumen().size(); i++)
			{
				maximo.push_back(Imagen::fila_t());
				for (int j = 0; j < this->get_alto(); j++)
				{
					maximo[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada fila
				for (int i = 0; i < pixeles.size(); i++)
				{
					Imagen::elemento_t min_val = copiar.get_max_tam();
					// Por cada elemento
					for (int j = 0; j < pixeles[i].size(); j++)
					{
						if (pixeles[i][j] < min_val)
						{
							min_val = pixeles[i][j];
						}
					}

					maximo[imagen][i] = min_val;
				}

				imagenes.pop();
			}

			proyeccion_imagen.set_pixeles(maximo);
		}
	}

	else if (direccion == 'z')
	{
		proyeccion_imagen.set_alto(this->get_volumen().size());
		proyeccion_imagen.set_ancho(this->get_ancho());

		if (criterio == "promedio")
		{
			// Crear una matriz
			Imagen::matriz_t promedio;
			for (int i = 0; i < this->get_volumen().size(); i++)
			{
				promedio.push_back(Imagen::fila_t());
				for (int j = 0; j < this->get_ancho(); j++)
				{
					promedio[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada columna
				for (int j = 0; j < actual.get_ancho(); j++)
				{
					Imagen::elemento_t acumulador = 0;

					// Por cada fila
					for (int i = 0; i < pixeles.size(); i++)
					{
						acumulador += pixeles[i][j];
					}

					acumulador = acumulador / this->get_ancho();

					promedio[imagen][j] = acumulador;
				}

				imagenes.pop();
			}

			proyeccion_imagen.set_pixeles(promedio);
		}

		else if (criterio == "maximo")
		{
			// Crear una matriz
			Imagen::matriz_t maximo;
			for (int i = 0; i < this->get_volumen().size(); i++)
			{
				maximo.push_back(Imagen::fila_t());
				for (int j = 0; j < this->get_ancho(); j++)
				{
					maximo[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada fila
				for (int j = 0; j < actual.get_ancho(); j++)
				{
					Imagen::elemento_t max_val = 0;
					// Por cada elemento
					for (int i = 0; i < pixeles.size(); i++)
					{
						if (pixeles[i][j] > max_val)
						{
							max_val = pixeles[i][j];
						}
					}

					maximo[imagen][j] = max_val;
				}

				imagenes.pop();
			}

			proyeccion_imagen.set_pixeles(maximo);
		}

		else if (criterio == "minimo")
		{
			// Crear una matriz
			Imagen::matriz_t minimo;
			for (int i = 0; i < this->get_volumen().size(); i++)
			{
				minimo.push_back(Imagen::fila_t());
				for (int j = 0; j < this->get_ancho(); j++)
				{
					minimo[i].push_back(copiar.get_max_tam());
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = this->get_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.get_pixeles();

				// Por cada fila
				for (int j = 0; j < actual.get_ancho(); j++)
				{
					Imagen::elemento_t min_val = copiar.get_max_tam();
					// Por cada elemento
					for (int i = 0; i < pixeles.size(); i++)
					{
						if (pixeles[i][j] < min_val)
						{
							min_val = pixeles[i][j];
						}
					}

					minimo[imagen][j] = min_val;
				}

				imagenes.pop();
			}

			proyeccion_imagen.set_pixeles(minimo);
		}
	}

	// Guardar la imagen
	proyeccion_imagen.guardarArchivo(nombre_archivo);
}