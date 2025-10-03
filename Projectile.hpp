#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <vector>
#include <ncurses.h>
#include "Screen.hpp"

struct Projectile
{
    int ownerId;
    int x, y;
    bool activo;

    Projectile(int startX, int startY, int playerId) : x(startX), y(startY), ownerId(playerId), activo(true) {}
    int getX() const { return (int)x; }
    int getY() const { return (int)y; }
    void desactivar() { activo = false; }
    bool estaActivo() const { return activo; }
    int getOwnerId() const { return ownerId; }
};

class ProjectileManager
{
private:
    std::vector<Projectile> proyectiles;

public:
    void disparar(int x, int y, int playerId); // agregar un proyectil
    void update(const Screen &screen);         // mover y eliminar inactivos
    void draw() const;                         // dibujar proyectiles
    std::vector<Projectile> &getProyectiles() { return proyectiles; }
};

#endif
