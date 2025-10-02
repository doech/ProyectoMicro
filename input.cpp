#include <ncurses.h>
#include "input.hpp"

std::set<int> teclasActivas;

void actualizarInput() {
    teclasActivas.clear();
    int ch;
    while ((ch = getch()) != ERR) {
        teclasActivas.insert(ch);
    }
}

bool estaPresionada(int key) {
    return teclasActivas.find(key) != teclasActivas.end();
}
