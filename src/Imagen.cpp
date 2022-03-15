#include "Imagen.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

Imagen::Imagen(std::string formato, matriz_t matriz_pixeles)
	: formato_imagen(formato), matriz_pixeles(matriz_pixeles)
{
	int valor_maximo = 0;

	// Verificar que la matriz sea correcta (Todos los vectores de adentro deben tener el mismo tamaño)
	for (matriz_t::iterator it1 = matriz_pixeles.begin(); it1 != matriz_pixeles.end(); it1++)
	{
		// Recorrer nuevamente la matriz de pixeles
		for (matriz_t::iterator it2 = matriz_pixeles.begin(); it2 != matriz_pixeles.end(); it2++)
		{
			// Si todos los vectores no tienen el mismo tamaño
			if (it1->size() != it2->size())
				throw std::exception();
		}

		// Por cada elemento de la fila
		for (fila_t::iterator ft = it1->begin(); ft != it1->end(); ft++)
		{
			if (*ft > valor_maximo)
				valor_maximo = *ft;
		}
	}

	this->max_tam = valor_maximo;
	this->alto = matriz_pixeles.size();
	this->ancho = matriz_pixeles[0].size();
}

Imagen::Imagen(std::string nombre_archivo) : nombre_archivo(nombre_archivo)
{
	// Verificar que el nombre_archivo termine por .pgm
	if (nombre_archivo.substr(nombre_archivo.find_last_of(".") + 1) != "pgm")
	{
		throw std::exception();
	}

	// Abrir el archivo
	std::fstream archivo(nombre_archivo);

	// Si no se pudo abrir
	if (!archivo.is_open())
	{
		throw std::exception();
	}

	int current_x = 0;
	int current_y = 0;

	bool creado = false;

	std::string linea;
	for (int i = 0; std::getline(archivo, linea); i++)
	{
		// Ignorar comentarios
		if (linea[0] == '#')
		{
			i--;
			continue;
		}

		// Interpretar
		if (i == 0) // Formato
		{
			this->formato_imagen = linea;
		}
		else if (i == 1) // Dimensiones
		{
			// Tokenizar, deben haber 2 números
			std::vector<int> enteros;
			std::stringstream linea2(linea);
			std::string temp;

			while (std::getline(linea2, temp, ' '))
				enteros.push_back(std::stoi(temp));

			// Guardar cada número ANCHO x ALTO
			this->ancho = enteros[0];
			this->alto = enteros[1];
		}
		else if (i == 2) // MaxTam
		{
			this->max_tam = std::stoi(linea);
		}
		else // Pixéles
		{
			if (creado == false)
			{
				// crear filas de tamaño ANCHO
				for (int i = 0; i < this->alto; i++)
				{
					this->matriz_pixeles.push_back(fila_t());
					for (int j = 0; j < this->ancho; j++)
					{
						this->matriz_pixeles[i].push_back(0);
					}
				}

				creado = true;
			}

			// Tokenizar
			std::stringstream linea2(linea);
			std::string temp;

			while (std::getline(linea2, temp, ' '))
			{
				this->matriz_pixeles[current_y][current_x] = std::stoi(temp);
				current_x++;

				if (current_x == this->ancho)
				{
					current_x = 0;
					current_y++;
				}
			}
		}
	}

	// Cerrar el archivo
	archivo.close();
}

bool Imagen::guardarArchivo(std::string nombre_archivo)
{
	// Abrir el archivo (crearlo si no existe)
	std::ofstream salida(nombre_archivo, std::ios::out);

	if (!salida.is_open())
		return false;

	// Guardar la cabecera
	salida << this->formato_imagen << '\n'					   // Formato
		   << "# Archivo generado por ProyeccionesEDD" << '\n' // Comentario generado por el sistema
		   << this->ancho << ' ' << this->alto << '\n'		   // Ancho x Alto
		   << this->max_tam << '\n';						   // Tamaño máximo

	// Por cada fila
	for (matriz_t::iterator it_fila = this->matriz_pixeles.begin();
		 it_fila != this->matriz_pixeles.end(); it_fila++)
	{
		// Por cada elemento de la fila
		for (fila_t::iterator it_pixel = it_fila->begin();
			 it_pixel != it_fila->end(); it_pixel++)
		{
			salida << *it_pixel << ' ';
		}
		salida << '\n';
	}

	// Cerrar el archivo
	salida.close();
	return true;
}

std::string Imagen::obtener_formato() const
{
	return formato_imagen;
}
void Imagen::fijar_formato(const std::string &formatoImagen) { formato_imagen = formatoImagen; }

int Imagen::obtener_ancho() const { return ancho; }
void Imagen::fijar_ancho(int ancho_) { ancho = ancho_; }

int Imagen::obtener_alto() const { return alto; }
void Imagen::fijar_alto(int alto_) { alto = alto_; }

int Imagen::obtener_max_tam() const { return max_tam; }
void Imagen::fijar_max_tam(int maxTam) { max_tam = maxTam; }

Imagen::matriz_t Imagen::obtener_pixeles() const { return matriz_pixeles; }
void Imagen::fijar_pixeles(Imagen::matriz_t matrizPixeles) { matriz_pixeles = matrizPixeles; }

std::string Imagen::obtener_nombre_archivo() const
{
	return this->nombre_archivo;
}

void Imagen::fijar_nombre_archivo(std::string nombre)
{
	this->nombre_archivo = nombre;
}

std::string Imagen::to_string()
{
	return "(proceso satisfactorio) Imagen cargada en memoria: " + nombre_archivo +
		   ", ancho: " + std::to_string(ancho) +
		   ", alto: " + std::to_string(alto);
}