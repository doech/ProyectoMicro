#ifndef INPUT_HPP
#define INPUT_HPP

#include <set>

extern std::set<int> teclasActivas;

void actualizarInput();   // actualiza las teclas activas del frame
bool estaPresionada(int key); // consulta si una tecla está activa

#endif
