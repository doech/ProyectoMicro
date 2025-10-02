#include "Projectile.hpp"
#include <algorithm> 

void ProjectileManager::disparar(int x, int y) {
    proyectiles.emplace_back(x, y - 1); // justo arriba de la nave
}

void ProjectileManager::update(const Screen &screen) {
    for (auto &p : proyectiles) {
        if (p.activo) {
            p.y--;
            if (p.y <= screen.top) {
                p.activo = false;
            }
        }
    }

    // Eliminar proyectiles inactivos
    proyectiles.erase(
        std::remove_if(proyectiles.begin(), proyectiles.end(),
                       [](const Projectile &p) { return !p.activo; }),
        proyectiles.end());
}

void ProjectileManager::draw() const {
    for (const auto &p : proyectiles) {
        if (p.activo) {
            mvprintw(p.y, p.x, "|");
        }
    }
}
