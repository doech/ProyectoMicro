#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include <vector>
#include <ncurses.h>
#include "Screen.hpp"
#include <vector>
#include <memory>
#include <pthread.h>

class Asteroid
{
private:
    pthread_t hilo;
    const Screen *screen;
    double x, y;
    double dx, dy;
    char symbol;
    enum Estado
    {
        VIVO,
        DESTRUYENDO,
        MUERTO
    } estado;
    int framesRestantes;

public:
    Asteroid(double startX, double startY, double dirX, double dirY, const Screen &s, char sym = 'O')
        : x(startX), y(startY), dx(dirX), dy(dirY), screen(&s), symbol(sym), estado(VIVO), framesRestantes(0) {}

    static void *ciclo(void *arg);
    void iniciar();

    void mover();
    void dibujar() const;

    bool estaActivo() const { return estado == VIVO; }
    void destruir()
    {
        estado = DESTRUYENDO;
        framesRestantes = 5;
    } // 5 frames de "explosión"
    void eliminar()
    {
        estado = MUERTO;
    }

    void detener()
    {
        pthread_join(hilo, nullptr);
    }

    int getX() const { return (int)x; }
    int getY() const { return (int)y; }
    double getDx() const { return dx; }
    double getDy() const { return dy; }
    char getSymbol() const { return symbol; }
};

class AsteroidManager
{
private:
    const Screen *screen;
    std::vector<std::unique_ptr<Asteroid>> asteroides;

public:
    AsteroidManager(const Screen &s) : screen(&s) {}

    void spawn(double x, double y, double dx, double dy, char sym = 'O');
    void update();
    void draw() const;
    const std::vector<std::unique_ptr<Asteroid>> &getAsteroides() const { return asteroides; }
};

#endif
