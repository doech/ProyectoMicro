#include <ncurses.h>
#include "Game.hpp"
#include "input.hpp"
#include "Screen.hpp"
#include "Projectile.hpp"
#include <algorithm>

Game::Game() : jugando(true), player((screen.right - screen.left) / 2, screen.bottom - 1), screen()
{
    getmaxyx(stdscr, max_y, max_x);
    nodelay(stdscr, TRUE);
}

void Game::procesarInput()
{
    if (estaPresionada('q'))
    {
        jugando = false;
    }
    if (estaPresionada(' '))
    {
        projectileManager.disparar(player.getX(), player.getY());
    }
    if (estaPresionada('w'))
        player.mover('w', screen);
    if (estaPresionada('s'))
        player.mover('s', screen);
    if (estaPresionada('a'))
        player.mover('a', screen);
    if (estaPresionada('d'))
        player.mover('d', screen);
}

void Game::update()
{
    if(player.getVidas() <= 0 ){ jugando = false; }
    checkCollisions();
    player.update();
    projectileManager.update(screen);
    asteroidManager.update(screen);
}

void Game::draw()
{
    screen.clearScreen();

    screen.drawHUD(player.getVidas(), puntaje, modo);
    screen.drawBorders();

    
    projectileManager.draw(); // dibuja los proyectiles
    asteroidManager.draw();   // dibuja los asteroides
    player.dibujar();         // dibuja la nave/jugador

    screen.drawMessage("Presione Q para salir");
    screen.refreshScreen();
}

void Game::checkCollisions()
{
    auto &proyectiles = projectileManager.getProyectiles();
    auto &asteroides = asteroidManager.getAsteroides();

    for (auto &p : proyectiles)
    {
        if (!p.estaActivo())
            continue;

        for (auto &a : asteroides)
        {
            if (!a.estaActivo())
                continue;

            // 🚀 Colisión: misma celda en la grilla
            if (p.getX() == a.getX() && p.getY() == a.getY())
            {
                if(a.getSymbol() == 'O'){
                    asteroidManager.spawn(a.getX(), a.getY() + 1, -a.getDx(), a.getDy(), '*');
                    asteroidManager.spawn(a.getX() + 1, a.getY() + 1, a.getDx(), a.getDy(), '*');
                }
                p.desactivar();
                a.destruir();
                puntaje += 10; // suma puntos al destruir asteroide
            }
        }
    }

    for (auto &a : asteroidManager.getAsteroides())
    {
        if (!a.estaActivo())
            continue;

        if (player.getX() == a.getX() && player.getY() == a.getY())
        {
            player.perderVida();
            a.eliminar(); // el asteroide también se destruye
        }
    }

    // 🔧 Limpieza: eliminar proyectiles inactivos
    proyectiles.erase(
        std::remove_if(proyectiles.begin(), proyectiles.end(),
                       [](const Projectile &p)
                       { return !p.estaActivo(); }),
        proyectiles.end());

    // 🔧 Limpieza: eliminar asteroides inactivos
    asteroides.erase(
        std::remove_if(asteroides.begin(), asteroides.end(),
                       [](const Asteroid &a)
                       { return !a.estaActivo(); }),
        asteroides.end());
}

void Game::run()
{
    asteroidManager.spawn(screen.right / 2, screen.top + 5, 0.1, 0.1, '*');  // ejemplo
    asteroidManager.spawn(screen.right / 8, screen.top + 5, 0.1, 0.05, 'O'); // ejemplo
    asteroidManager.spawn(screen.right / 4, screen.top + 3, 0.1, 0.05, '*'); // ejemplo
    asteroidManager.spawn(screen.right / 5, screen.top + 4, 0.1, 0.05, 'O'); // ejemplo
    asteroidManager.spawn(screen.right - 5, screen.top + 1, 0, 0.1, 'O');  // ejemplo
    while (jugando)
    {
        actualizarInput();
        procesarInput();
        update();
        draw();
        napms(50);
    }
    nodelay(stdscr, FALSE);

    
    if (!player.estaVivo()) {
        screen.drawGameOver(puntaje, nombreJugador);
    }
}
