#include "Asteroid.hpp"
#include <algorithm>
#include "Screen.hpp"
#include <pthread.h>
#include <unistd.h>
#include <mutex>


void Asteroid::mover()
{
    if (estado == MUERTO)
        return;

    if (estado == DESTRUYENDO)
    {
        framesRestantes--;
        if (framesRestantes <= 0)
        {
            eliminar();
        }
        return; // no se mueve mientras se destruye
    }

    x += dx;
    y += dy;

    // Si toca bordes laterales → rebota
    if (x <= screen->left + 1 || x >= screen->right - 1)
        dx = -dx;

    if (y <= screen->top + 1)
        y = screen->bottom - 1;

    if (y >= screen->bottom - 1)
        y = screen->top + 1;
}

void Asteroid::dibujar() const
{
    if (estado == MUERTO)
        return;

    char c = symbol;
    if (estado == DESTRUYENDO)
        c = 'X'; // 🚀 caracter de explosión

    mvprintw((int)y, (int)x, "%c", c);
}

void* Asteroid::ciclo(void* arg) {
    Asteroid* a = (Asteroid*)arg;

    while (a->estado.load() != Estado::MUERTO) {
        a->mover();
        usleep(50000);
    }

    return nullptr;
}

// ---------------- Manager ----------------
void AsteroidManager::spawn(double x, double y, double dx, double dy, char sym)
{
    auto a = std::make_unique<Asteroid>(x, y, dx, dy, *screen, sym);
    a->iniciar(); // lanza el hilo
    asteroides.push_back(std::move(a));
}

void AsteroidManager::update() {
    auto it = asteroides.begin();
    while (it != asteroides.end()) {
        auto& asteroid = *it;

        if (!asteroid->estaActivo()) {
            it = asteroides.erase(it);
        } else {
            ++it;
        }
    }
}

void AsteroidManager::draw() const
{
    for (const auto &a : asteroides)
    {
        a->dibujar();
    }
}
