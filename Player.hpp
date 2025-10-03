#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <ncurses.h>
#include <vector>
#include "Screen.hpp"
#include <pthread.h>
#include <atomic>
#include <list>
#include <memory>
#include "input.hpp"
#include "Projectile.hpp"

struct Controls
{
    int up;
    int down;
    int left;
    int right;
    int shoot;
}; // se usan ints porque ncourses usa numeros para identificar teclas

enum Direccion
{
    NONE,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class Player
{
private:
    static int nextId; // contador global de IDs
    int id;            // id único de este jugador
    std::string nombre;
    int puntaje;
    pthread_t hilo;
    int x, y;
    std::atomic<int> vidas;
    std::atomic<int> invulnerableFrames;
    const Screen *screen;
    Controls controls;
    std::atomic<Direccion> direccion{NONE};
    ProjectileManager *projectileManager;

public:
    Player(int startX, int startY, Controls c, const Screen &s, ProjectileManager &pm) : x(startX), y(startY), controls(c), screen(&s), projectileManager(&pm), vidas(3), invulnerableFrames(0)
    {
        id = nextId++;
        puntaje = 0;
    }
    int getPuntaje() const { return puntaje; }
    std::string getNombre() const { return nombre; }
    int getId() const { return id; }
    static void *ciclo(void *arg);
    void sumarPuntaje(int puntos)
    {
        puntaje += puntos;
    }
    void iniciar()
    {
        pthread_create(&hilo, nullptr, ciclo, this);
    }

    void mover();

    void setNombre(std::string n)
    {
        nombre = n;
    }

    void setDireccion(int tecla)
    {
        if (tecla == controls.up)
            direccion = UP;
        else if (tecla == controls.down)
            direccion = DOWN;
        else if (tecla == controls.left)
            direccion = LEFT;
        else if (tecla == controls.right)
            direccion = RIGHT;
        else
            direccion = NONE;
    }

    void draw() const;

    int getX() const { return x; }
    int getY() const { return y; }
    int getVidas() const { return vidas; }

    void perderVida()
    {
        if (invulnerableFrames == 0 && vidas > 0)
        {
            vidas--;
            invulnerableFrames.store(50); // medio segundo si cada frame ~25ms
        }
    }

    void update()
    {
        if (invulnerableFrames > 0)
            invulnerableFrames--;

        if (estaPresionada(controls.up) && y > screen->top + 1)
            y--;
        if (estaPresionada(controls.down) && y < screen->bottom - 1)
            y++;
        if (estaPresionada(controls.left) && x > screen->left + 1)
            x--;
        if (estaPresionada(controls.right) && x < screen->right - 1)
            x++;

        if (estaPresionada(controls.shoot))
        {
            projectileManager->disparar(x, y, id);
        }
    }
    bool estaVivo() const { return vidas.load() > 0; }
    bool estaInvulnerable() const { return invulnerableFrames > 0; }

    ~Player()
    {
        if (hilo)
        {
            vidas.store(0); // asegurarse que salga del ciclo
            pthread_join(hilo, nullptr);
        }
    } // destructor
};

//---------Manager---------

class PlayerManager
{
private:
    const Screen *screen;
    std::list<std::unique_ptr<Player>> players;
    ProjectileManager *projectileManager;

public:
    PlayerManager(const Screen &s, ProjectileManager &pm) : screen(&s), projectileManager(&pm) {}

    void spawn(double x, double y, Controls c);
    void update();
    bool allPlayersDied()
    {
        for (auto &p : players)
        {
            if (p->estaVivo())
            {
                return false; // encontramos al menos uno vivo
            }
        }
        return true; // ninguno vivo
    }
    int getTotalScore() const
    {
        int total = 0;
        for (auto &p : players)
        {
            total += p->getPuntaje();
        }
        return total;
    }
    void draw() const;
    const std::list<std::unique_ptr<Player>> &getPlayers() const { return players; }
};

#endif
