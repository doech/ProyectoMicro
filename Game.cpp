#include <ncurses.h>
#include "Game.hpp"
#include "input.hpp"
#include "Screen.hpp"
#include "Projectile.hpp"
#include <algorithm>
#include <pthread.h>


Game::Game(int modoSeleccionado)
    : jugando(true), modo(modoSeleccionado),
      player((screen.right - screen.left) / 2, screen.bottom - 1),
      screen()
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
    if (player.getVidas() <= 0)
    {
        jugando = false;
    }
    player.update();
    projectileManager.update(screen);
    asteroidManager.update(screen);
    checkCollisions();
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
                if (a.getSymbol() == 'O')
                {
                    int newX1 = std::clamp((int)a.getX(), screen.left + 1, screen.right - 2);
                    int newX2 = std::clamp((int)a.getX() + 1, screen.left + 1, screen.right - 2);
                    int newY = std::clamp((int)a.getY() + 1, screen.top + 1, screen.bottom - 2);

                    asteroidManager.spawn(newX1, newY, -a.getDx()*0.9, a.getDy()*0.9, '*');
                    asteroidManager.spawn(newX2, newY, a.getDx()*1.1, a.getDy()*1.1, '*');
                }
                p.desactivar();
                a.destruir();
                if (a.getSymbol() == '*') puntaje += 10; // suma puntos al destruir asteroide
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
}

void Game::run()
{
    // Configuración inicial según modo
    if (modo == 1)
    {
        // Modo 1: 3 asteroides grandes, velocidad media
        asteroidManager.spawn(screen.right / 3, screen.top + 3, 0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 2, screen.top + 5, -0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 4, screen.top + 4, 0.1, 0.05, 'O');
    }
    else if (modo == 2)
    {
        // Modo 2: 5 asteroides grandes, velocidad alta
        asteroidManager.spawn(screen.right / 3, screen.top + 3, 0.2, 0.1, 'O');
        asteroidManager.spawn(screen.right / 2, screen.top + 5, -0.2, 0.1, 'O');
        asteroidManager.spawn(screen.right / 4, screen.top + 4, 0.2, 0.1, 'O');
        asteroidManager.spawn(screen.right / 6, screen.top + 6, 0.15, 0.12, 'O');
        asteroidManager.spawn(screen.right - 6, screen.top + 2, -0.15, 0.12, 'O');
    }
    else if (modo == 3)
    {
        // Modo 3: 2 jugadores (por ahora dejamos como "infinito" con asteroides normales)
        asteroidManager.spawn(screen.right / 3, screen.top + 3, 0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 2, screen.top + 5, -0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 4, screen.top + 4, 0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 5, screen.top + 6, -0.1, 0.05, 'O');
    }

    while (jugando)
    {
        actualizarInput();
        procesarInput();
        update();
        draw();
        napms(50);

        // Condiciones de victoria según modo
        if (modo == 1 && puntaje >= 60)
        { // 6 asteroides pequeños * 10 pts
            jugando = false;
        }
        else if (modo == 2 && puntaje >= 100)
        { // 10 asteroides pequeños * 10 pts
            jugando = false;
        }
        // Modo 3: infinito, solo termina si mueren jugadores
    }

    nodelay(stdscr, FALSE);

    if (!player.estaVivo())
    {
        screen.drawGameOver(puntaje, nombreJugador);
    }
}
