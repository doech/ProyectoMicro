#include "Asteroid.hpp"

Asteroid::Asteroid(double startX, double startY, double dirX, double dirY, char sym)
    : x(startX), y(startY), dx(dirX), dy(dirY),
      symbol(sym), estado(VIVO), framesRestantes(0) {}

void Asteroid::mover(const Screen &screen)
{
    if (estado == MUERTO)
        return;

    if (estado == DESTRUYENDO)
    {
        framesRestantes--;
        if (framesRestantes <= 0)
        {
            estado = MUERTO;
        }
        return; // no se mueve mientras se destruye
    }

    x += dx;
    y += dy;

    // Si toca bordes laterales → rebota
    if (x <= screen.left + 1 || x >= screen.right - 1)
        dx = -dx;

    if (y <= screen.top + 1)
        y = screen.bottom - 1;

    if (y >= screen.bottom - 1)
        y = screen.top + 1;
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

// ---------------- Manager ----------------
void AsteroidManager::spawn(double x, double y, double dx, double dy, char sym)
{
    asteroides.emplace_back(x, y, dx, dy, sym);
}

void AsteroidManager::update(const Screen &screen)
{
    for (auto &a : asteroides)
    {
        a.mover(screen);
    }
}

void AsteroidManager::draw() const
{
    for (const auto &a : asteroides)
    {
        a.dibujar();
    }
}
