# Compiler and Standard
CC	= @g++
STD	= c++14

# Directories
OUT	= ./build
SRC	= ./src
TST = ./test

# Compiler options
WARNL = -Wall
OPT = -O0

# Flags
FLAGS = -std=$(STD) $(WARNL) $(OPT) -g

all: $(OUT)/main

# Targets
TARGETS = $(OUT)/Consola.o $(OUT)/Imagen.o $(OUT)/Volumen.o $(OUT)/Controlador.o

# Compilacion
$(OUT)/Consola.o: $(SRC)/Consola.cpp $(SRC)/Consola.h
	$(CC) -c $(FLAGS) $< -o $@

$(OUT)/Controlador.o: $(SRC)/Controlador.cpp $(SRC)/Controlador.h
	$(CC) -c $(FLAGS) $< -o $@

$(OUT)/Imagen.o: $(SRC)/Imagen.cpp $(SRC)/Imagen.h
	$(CC) -c $(FLAGS) $< -o $@

$(OUT)/Volumen.o: $(SRC)/Volumen.cpp $(SRC)/Volumen.h
	$(CC) -c $(FLAGS) $< -o $@

$(OUT)/main: $(SRC)/main.cpp $(TARGETS)
	$(CC) $(FLAGS) $^ -o $@

# Eliminación
.PHONY: clean
clean:
	rm $(OUT)/*