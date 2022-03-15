#include "../src/Imagen.h"
#include <iostream>

int main() {
    Imagen imagen{"test.pgm"};
    std::cout << imagen.to_string();
}