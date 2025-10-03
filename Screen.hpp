#ifndef SCREEN_HPP
#define SCREEN_HPP

#include <ncurses.h>
#include <string>
#include <list>
#include <memory>

class Player;

class Screen {
private:
    int max_y, max_x;
public:
    int top, bottom, left, right; // límites del campo de juego

    Screen();

    void clearScreen();
    void refreshScreen();

     void drawHUD(const std::list<std::unique_ptr<Player>> &players, int modo);
    void drawBorders();
    void drawMessage(const std::string &msg);
    void drawEndScreen(const std::list<std::unique_ptr<Player>> &players, bool win);
    bool confirmacionSalir();
};

#endif
