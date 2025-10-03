#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include <vector>
#include <ncurses.h>
#include "Screen.hpp"
#include <vector>
#include <memory>
#include <pthread.h>
#include <atomic>
#include <list>

enum Estado
{
    VIVO,
    DESTRUYENDO,
    MUERTO
};

class Asteroid
{
private:
    pthread_t hilo;
    const Screen *screen;
    double x, y;
    double dx, dy;
    char symbol;

    std::atomic<Estado> estado;
    std::atomic<int> framesRestantes;

public:
    Asteroid(double startX, double startY, double dirX, double dirY, const Screen &s, char sym = 'O')
        : x(startX), y(startY), dx(dirX), dy(dirY), screen(&s), symbol(sym), estado(VIVO), framesRestantes(0) {}

    static void *ciclo(void *arg);
    void iniciar()
    {
        pthread_create(&hilo, nullptr, ciclo, this);
    }

    void mover();
    void dibujar() const;

    bool estaActivo() const { return estado != MUERTO; } // esta activo si el estado es VIVO ó DESTRUYENDO
    bool estaVivo() const { return estado == VIVO; }     // esta vivo si el estado es VIVO
    void destruir()
    {
        estado.store(Estado::DESTRUYENDO);
        framesRestantes.store(5);
    } // 5 frames de "explosión"
    void eliminar()
    {
        estado.store(Estado::MUERTO);
    }

    int getX() const { return (int)x; }
    int getY() const { return (int)y; }
    double getDx() const { return dx; }
    double getDy() const { return dy; }
    char getSymbol() const { return symbol; }

    ~Asteroid()
    {
        if (hilo)
        {
            estado.store(MUERTO); // asegurarse que salga del ciclo
            pthread_join(hilo, nullptr);
        }
    } // destructor
};

class AsteroidManager
{
private:
    const Screen *screen;
    std::list<std::unique_ptr<Asteroid>> asteroides;

public:
    AsteroidManager(const Screen &s) : screen(&s) {}

    void spawn(double x, double y, double dx, double dy, char sym = 'O');
    void update();
    void draw() const;
    int getTotalAsteroids() const
    {
        return static_cast<int>(asteroides.size());
    }
    const std::list<std::unique_ptr<Asteroid>> &getAsteroides() const { return asteroides; }
};

#endif
