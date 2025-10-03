#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include "Player.hpp"
#include "Screen.hpp"
#include "Projectile.hpp"
#include "Asteroid.hpp"
#include <chrono>
#include "scores.hpp"

class Game
{
private:
    const int FPS = 20;
    Screen screen;
    bool jugando;
    int max_y, max_x;
    int modo = 1;
    std::string nombreJugador;
    ProjectileManager projectileManager;
    AsteroidManager asteroidManager;
    PlayerManager playerManager;
    bool returnToMenu;
    HighScores *HS;

public:
    Game(int modoSeleccionado, HighScores &hs)
        : jugando(true), modo(modoSeleccionado),
          playerManager(screen, projectileManager),
          screen(), asteroidManager(screen),
          HS(&hs)
    {
        getmaxyx(stdscr, max_y, max_x);
        nodelay(stdscr, TRUE);
    }
    void run();             // Loop principal
    void procesarInput();   // Procesa teclas activas
    void update();          // Actualiza estado (proyectiles, colisiones, etc.)
    void checkCollisions(); // Chequea colisiones entre cuerpos
    void draw();            // Dibuja todo en pantalla
    void registrarPuntaje(const std::string &nombreJugador, int puntosFinales)
    {
        long ts = std::time(nullptr); // epoch seconds (fecha/hora actuales)
        HS->add(nombreJugador, puntosFinales, ts);
        HS->save(); // persiste en scores.csv
    }

    std::string getNombreJugador() const { return nombreJugador; }
};

#endif
