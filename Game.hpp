#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include "Player.hpp"
#include "Screen.hpp"
#include "Projectile.hpp"
#include "Asteroid.hpp"
#include <chrono>

class Game
{
private:
    const int FPS = 20;
    Screen screen;
    bool jugando;
    int max_y, max_x;
    int puntaje = 0;
    int modo = 1;
    std::string nombreJugador;
    ProjectileManager projectileManager;
    AsteroidManager asteroidManager;
    PlayerManager playerManager;

public:
    Game(int modoSeleccionado); 
    void run();             // Loop principal
    void procesarInput();   // Procesa teclas activas
    void update();          // Actualiza estado (proyectiles, colisiones, etc.)
    void checkCollisions(); // Chequea colisiones entre cuerpos
    void draw();            // Dibuja todo en pantalla

    int getPuntajeFinal() const { return puntaje; }
    std::string getNombreJugador() const { return nombreJugador; }
};

#endif
