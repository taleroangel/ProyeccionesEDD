#include "Proyeccion.hxx"
#include "Imagen.hxx"

#include <iostream>

Proyeccion::Proyeccion(std::string criterio, char direccion, std::string nombre_archivo)
	: criterio(criterio), direccion(direccion), nombre_archivo(nombre_archivo) {}

void Proyeccion::crearProyeccion(Volumen volumen)
{
	// Copiar las propiedades de una imagen
	Imagen copiar = volumen.obtener_volumen().front();
	Imagen proyeccion_imagen{}; // Donde se guarda la proyección

	proyeccion_imagen.fijar_formato(copiar.obtener_formato());
	proyeccion_imagen.fijar_max_tam(copiar.obtener_max_tam());
	proyeccion_imagen.fijar_nombre_archivo(this->nombre_archivo);

	//* De arriba para abajo
	if (this->direccion == 'x')
	{
		proyeccion_imagen.fijar_alto(copiar.obtener_alto());
		proyeccion_imagen.fijar_ancho(copiar.obtener_ancho());

		if (this->criterio == "promedio")
		{
			// Crear una matriz
			Imagen::matriz_t promedio;
			for (int i = 0; i < copiar.obtener_alto(); i++)
			{
				promedio.push_back(Imagen::fila_t());
				for (int j = 0; j < copiar.obtener_ancho(); j++)
				{
					promedio[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

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
					promedio[i][j] = promedio[i][j] / copiar.obtener_alto();
				}
			}

			proyeccion_imagen.fijar_pixeles(promedio);
		}

		// Maximo
		else if (this->criterio == "maximo")
		{
			// Crear una matriz
			Imagen::matriz_t maximo;
			for (int i = 0; i < copiar.obtener_alto(); i++)
			{
				maximo.push_back(Imagen::fila_t());
				for (int j = 0; j < copiar.obtener_ancho(); j++)
				{
					maximo[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

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

			proyeccion_imagen.fijar_pixeles(maximo);
		}

		// Mínimo
		else if (this->criterio == "minimo")
		{
			// Crear una matriz
			Imagen::matriz_t minimo;
			for (int i = 0; i < copiar.obtener_alto(); i++)
			{
				minimo.push_back(Imagen::fila_t());
				for (int j = 0; j < copiar.obtener_ancho(); j++)
				{
					minimo[i].push_back(copiar.obtener_max_tam());
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

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

			proyeccion_imagen.fijar_pixeles(minimo);
		}
	}

	//* Desde el lado, de derecha a izquierda
	else if (this->direccion == 'y')
	{
		proyeccion_imagen.fijar_alto(volumen.obtener_volumen().size());
		proyeccion_imagen.fijar_ancho(volumen.obtener_alto());

		if (this->criterio == "promedio")
		{
			// Crear una matriz
			Imagen::matriz_t promedio;
			for (int i = 0; i < volumen.obtener_volumen().size(); i++)
			{
				promedio.push_back(Imagen::fila_t());
				for (int j = 0; j < volumen.obtener_alto(); j++)
				{
					promedio[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

				// Por cada fila
				for (int i = 0; i < pixeles.size(); i++)
				{
					Imagen::elemento_t acumulador = 0;
					// Por cada elemento
					for (int j = 0; j < pixeles[i].size(); j++)
					{
						acumulador += pixeles[i][j];
					}

					acumulador = acumulador / volumen.obtener_ancho();

					promedio[imagen][i] = acumulador;
				}

				imagenes.pop();
			}

			proyeccion_imagen.fijar_pixeles(promedio);
		}

		// Maximo
		else if (this->criterio == "maximo")
		{
			// Crear una matriz
			Imagen::matriz_t maximo;
			for (int i = 0; i < volumen.obtener_volumen().size(); i++)
			{
				maximo.push_back(Imagen::fila_t());
				for (int j = 0; j < volumen.obtener_alto(); j++)
				{
					maximo[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

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

			proyeccion_imagen.fijar_pixeles(maximo);
		}

		// Mínimo
		else if (this->criterio == "minimo")
		{
			// Crear una matriz
			Imagen::matriz_t maximo;
			for (int i = 0; i < volumen.obtener_volumen().size(); i++)
			{
				maximo.push_back(Imagen::fila_t());
				for (int j = 0; j < volumen.obtener_alto(); j++)
				{
					maximo[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

				// Por cada fila
				for (int i = 0; i < pixeles.size(); i++)
				{
					Imagen::elemento_t min_val = copiar.obtener_max_tam();
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

			proyeccion_imagen.fijar_pixeles(maximo);
		}
	}

	else if (this->direccion == 'z')
	{
		proyeccion_imagen.fijar_alto(volumen.obtener_volumen().size());
		proyeccion_imagen.fijar_ancho(volumen.obtener_ancho());

		if (this->criterio == "promedio")
		{
			// Crear una matriz
			Imagen::matriz_t promedio;
			for (int i = 0; i < volumen.obtener_volumen().size(); i++)
			{
				promedio.push_back(Imagen::fila_t());
				for (int j = 0; j < volumen.obtener_ancho(); j++)
				{
					promedio[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

				// Por cada columna
				for (int j = 0; j < actual.obtener_ancho(); j++)
				{
					Imagen::elemento_t acumulador = 0;

					// Por cada fila
					for (int i = 0; i < pixeles.size(); i++)
					{
						acumulador += pixeles[i][j];
					}

					acumulador = acumulador / volumen.obtener_ancho();

					promedio[imagen][j] = acumulador;
				}

				imagenes.pop();
			}

			proyeccion_imagen.fijar_pixeles(promedio);
		}

		else if (this->criterio == "maximo")
		{
			// Crear una matriz
			Imagen::matriz_t maximo;
			for (int i = 0; i < volumen.obtener_volumen().size(); i++)
			{
				maximo.push_back(Imagen::fila_t());
				for (int j = 0; j < volumen.obtener_ancho(); j++)
				{
					maximo[i].push_back(0);
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

				// Por cada fila
				for (int j = 0; j < actual.obtener_ancho(); j++)
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

			proyeccion_imagen.fijar_pixeles(maximo);
		}

		else if (this->criterio == "minimo")
		{
			// Crear una matriz
			Imagen::matriz_t minimo;
			for (int i = 0; i < volumen.obtener_volumen().size(); i++)
			{
				minimo.push_back(Imagen::fila_t());
				for (int j = 0; j < volumen.obtener_ancho(); j++)
				{
					minimo[i].push_back(copiar.obtener_max_tam());
				}
			}

			// Obtener todas las imágenes
			std::queue<Imagen> imagenes = volumen.obtener_volumen();
			for (int imagen = 0; imagen < imagenes.size(); imagen++)
			{
				// Por cada imagen
				Imagen actual = imagenes.front();
				// Obtener sus píxeles
				Imagen::matriz_t pixeles = actual.obtener_pixeles();

				// Por cada fila
				for (int j = 0; j < actual.obtener_ancho(); j++)
				{
					Imagen::elemento_t min_val = copiar.obtener_max_tam();
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

			proyeccion_imagen.fijar_pixeles(minimo);
		}
	}

	// Guardar la imagen
	proyeccion_imagen.guardarArchivo(this->nombre_archivo);
}

std::string Proyeccion::obtener_criterio() const
{
	return criterio;
}

void Proyeccion::fijar_criterio(std::string criterio)
{
	this->criterio = criterio;
}

char Proyeccion::obtener_direccion() const
{
	return direccion;
}
void Proyeccion::fijar_direccion(char direccion)
{
	this->direccion = direccion;
}

std::string Proyeccion::obtener_nombre_archivo() const
{
	return nombre_archivo;
}
void Proyeccion::fijar_nombre_archivo(std::string nombre_archivo)
{
	this->nombre_archivo = nombre_archivo;
}