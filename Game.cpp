#include <ncurses.h>
#include "Game.hpp"
#include "input.hpp"
#include "Screen.hpp"
#include "Projectile.hpp"
#include <algorithm>
#include <pthread.h>
#include "scores.hpp"

void Game::procesarInput()
{
    if (estaPresionada('q'))
    {
        if (screen.confirmacionSalir()) // si devuelve true, salir
        {
            returnToMenu = true;
        }
    }
}

void Game::update()
{
    if (playerManager.allPlayersDied())
    {
        jugando = false;
    }
    playerManager.update();
    projectileManager.update(screen);
    asteroidManager.update();
    checkCollisions();
}

void Game::draw()
{
    screen.clearScreen();

    screen.drawHUD(playerManager.getPlayers(), modo);
    screen.drawBorders();

    projectileManager.draw(); // dibuja los proyectiles
    asteroidManager.draw();   // dibuja los asteroides
    playerManager.draw();     // dibuja la nave/jugador

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
            if (!a->estaVivo())
                continue;

            // 🚀 Colisión: misma celda en la grilla
            if (p.getX() == a->getX() && p.getY() == a->getY())
            {
                if (a->getSymbol() == 'O')
                {
                    int newX1 = std::clamp((int)a->getX(), screen.left + 1, screen.right - 2);
                    int newX2 = std::clamp((int)a->getX() + 1, screen.left + 1, screen.right - 2);
                    int newY = std::clamp((int)a->getY() + 1, screen.top + 1, screen.bottom - 2);

                    asteroidManager.spawn(newX1, newY, -a->getDx() * 0.9, a->getDy() * 0.9, '*');
                    asteroidManager.spawn(newX2, newY, a->getDx() * 1.1, a->getDy() * 1.1, '*');
                }
                p.desactivar();
                a->destruir();
                if (a->getSymbol() == '*')
                {
                    int owner = p.getOwnerId();
                    for (auto &player : playerManager.getPlayers())
                    {
                        if (player->getId() == owner)
                        {
                            player->sumarPuntaje(10);
                        }
                    }
                }
            }
        }
    }

    for (auto &p : playerManager.getPlayers()) // 🔹 recorrer cada jugador
    {
        if (!p->estaVivo())
            continue;

        for (auto &a : asteroidManager.getAsteroides()) // 🔹 recorrer cada asteroide
        {
            if (!a->estaVivo())
                continue;

            if (p->getX() == a->getX() && p->getY() == a->getY())
            {
                p->perderVida();
                // a->eliminar(); // el asteroide también se destruye
            }
        }
    }
}

void Game::run()
{

    keypad(stdscr, TRUE);  // habilita flechas y teclas especiales
    nodelay(stdscr, TRUE); // getch() no bloquea
    cbreak();              // entrada sin buffer
    noecho();              // no mostrar teclas

    // Configuración inicial según modo
    if (modo == 1)
    {
        playerManager.spawn((screen.right - screen.left) / 2, screen.bottom - 1, {'w', 's', 'a', 'd', ' '});
        // Modo 1: 3 asteroides grandes, velocidad media
        asteroidManager.spawn(screen.right / 3, screen.top + 3, 0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 2, screen.top + 5, -0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 4, screen.top + 4, 0.1, 0.05, 'O');
    }
    else if (modo == 2)
    {
        playerManager.spawn((screen.right - screen.left) / 2, screen.bottom - 1, {'w', 's', 'a', 'd', ' '});
        // Modo 2: 5 asteroides grandes, velocidad alta
        asteroidManager.spawn(screen.right / 3, screen.top + 3, 0.2, 0.1, 'O');
        asteroidManager.spawn(screen.right / 2, screen.top + 5, -0.2, 0.1, 'O');
        asteroidManager.spawn(screen.right / 4, screen.top + 4, 0.2, 0.1, 'O');
        asteroidManager.spawn(screen.right / 6, screen.top + 6, 0.15, 0.12, 'O');
        asteroidManager.spawn(screen.right - 6, screen.top + 2, -0.15, 0.12, 'O');
    }
    else if (modo == 3)
    {
        playerManager.spawn((screen.right - screen.left) / 4, screen.bottom - 1, {'w', 's', 'a', 'd', ' '});
        playerManager.spawn(screen.right - (screen.right - screen.left) / 4, screen.bottom - 1, {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, '\n'});
        // Modo 3: 2 jugadores (por ahora dejamos como "infinito" con asteroides normales)
        asteroidManager.spawn(screen.right / 3, screen.top + 3, 0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 2, screen.top + 5, -0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 4, screen.top + 4, 0.1, 0.05, 'O');
        asteroidManager.spawn(screen.right / 5, screen.top + 6, -0.1, 0.05, 'O');
    }

    returnToMenu = false;

    while (true)
    {

        actualizarInput();
        procesarInput();
        if (returnToMenu)
        {
            nodelay(stdscr, FALSE);
            return; // salir directo al menú si presionan q
        }
        update();
        draw();
        napms(50);

        if (asteroidManager.getTotalAsteroids() <= 0)
        {
            jugando = false;
        }
    }

    nodelay(stdscr, FALSE);

    if (playerManager.allPlayersDied())
    {
        screen.drawEndScreen(playerManager.getPlayers(), false);
    }
    else
    {
        screen.drawEndScreen(playerManager.getPlayers(), true);
    }

    for (auto &p : playerManager.getPlayers())
    {
        registrarPuntaje(p->getNombre(), p->getPuntaje());
    }
}
