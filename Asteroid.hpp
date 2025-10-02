#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include <vector>
#include <ncurses.h>
#include "Screen.hpp"

class Asteroid {
private:
    double x, y;
    double dx, dy;
    char symbol;
    enum Estado { VIVO, DESTRUYENDO, MUERTO } estado;
    int framesRestantes;

public:
    Asteroid(double startX, double startY, double dirX, double dirY, char sym = 'O');

    void mover(const Screen &screen);
    void dibujar() const;

    
bool estaActivo() const { return estado == VIVO; }
    void destruir() { estado = DESTRUYENDO; framesRestantes = 5; } // 5 frames de "explosión"
    void eliminar() { estado = MUERTO; }
    int getX() const { return (int)x; }
    int getY() const { return (int)y; }
    double getDx() const { return dx; }
    double getDy() const { return dy; }
    char getSymbol() const { return symbol; }

    
};

class AsteroidManager
{
private:
    std::vector<Asteroid> asteroides;

public:
    void spawn(double x, double y, double dx, double dy, char sym = 'O');
    void update(const Screen &screen);
    void draw() const;
    std::vector<Asteroid>& getAsteroides() { return asteroides; }
};

#endif
