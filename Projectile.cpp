#include "Projectile.hpp"
#include <algorithm>

void ProjectileManager::disparar(int x, int y, int playerId)
{
    proyectiles.emplace_back(x, y - 1, playerId); // justo arriba de la nave
}

void ProjectileManager::update(const Screen &screen)
{
    // Eliminar proyectiles inactivos
    proyectiles.erase(
        std::remove_if(proyectiles.begin(), proyectiles.end(),
                       [](const Projectile &p)
                       { return !p.activo; }),
        proyectiles.end());

    for (auto &p : proyectiles)
    {
        if (p.activo)
        {
            p.y--;
            if (p.y <= screen.top)
            {
                p.activo = false;
            }
        }
    }
}

void ProjectileManager::draw() const
{
    for (const auto &p : proyectiles)
    {
        if (p.activo)
        {
            mvprintw(p.y, p.x, "|");
        }
    }
}
