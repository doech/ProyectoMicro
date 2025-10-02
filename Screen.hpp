#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <ncurses.h>
#include <string>

class Screen {
private:
    int max_y, max_x;
public:
    int top, bottom, left, right; // límites del campo de juego

    Screen();

    void clearScreen();
    void refreshScreen();

    void drawHUD(int vidas, int puntaje, int modo);
    void drawBorders();
    void drawMessage(const std::string &msg);
    void drawGameOver(int puntaje, std::string &nombreJugador);
};

#endif
