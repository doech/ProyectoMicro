#include "Screen.hpp"

Screen::Screen() {
    getmaxyx(stdscr, max_y, max_x);

    // Definir límites jugables
    top = 3;
    bottom = max_y - 3;
    left = 1;
    right = max_x - 2;
}

void Screen::clearScreen() {
    clear();
}

void Screen::refreshScreen() {
    refresh();
}

void Screen::drawHUD(int vidas, int puntaje, int modo) {
    // Barra superior
    mvprintw(1, 2, "Vidas: ");
    for (int i = 0; i < vidas; i++) {
        mvaddch(1, 10 + i*2, ACS_DIAMOND);
    }

    mvprintw(1, max_x/2 - 7, "Puntaje: %03d", puntaje);
    mvprintw(1, max_x - 12, "Modo: %d", modo);

    // Línea divisoria
    for (int x = 0; x < max_x; x++) {
        mvprintw(2, x, "-");
    }
}

void Screen::drawBorders() {
    // Bordes izquierdo y derecho
    for (int y = top; y < bottom; y++) {
        mvprintw(y, 0, "|");
        mvprintw(y, max_x - 1, "|");
    }

    // Borde inferior
    for (int x = 0; x < max_x; x++) {
        mvprintw(bottom, x, "-");
    }
}

void Screen::drawMessage(const std::string &msg) {
    mvprintw(max_y - 2, 2, "%s", msg.c_str());
}

void Screen::drawGameOver(int puntaje, std::string &nombreJugador) {
    clear();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    std::string msg = "GAME OVER";
    std::string score = "Puntaje final: " + std::to_string(puntaje);

    mvprintw(max_y / 2 - 2, (max_x - msg.size()) / 2, "%s", msg.c_str());
    mvprintw(max_y / 2, (max_x - score.size()) / 2, "%s", score.c_str());

    mvprintw(max_y / 2 + 2, (max_x - 20) / 2, "Ingrese su nombre: ");
    echo(); // permite mostrar lo que se escribe
    char buffer[50];
    getnstr(buffer, 49); // leer hasta 49 caracteres
    noecho();
    nombreJugador = buffer;

    refresh();
}

