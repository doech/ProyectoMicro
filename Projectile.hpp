#ifndef PROJECTILE_HPP
#define PROJECTILE_HPP

#include <vector>
#include <ncurses.h>
#include "Screen.hpp"

struct Projectile
{
    int x, y;
    bool activo;

    Projectile(int startX, int startY) : x(startX), y(startY), activo(true) {}
    int getX() const { return (int)x; }
    int getY() const { return (int)y; }
    void desactivar() { activo = false; }
    bool estaActivo() const { return activo; }
};

class ProjectileManager
{
private:
    std::vector<Projectile> proyectiles;

public:
    void disparar(int x, int y);       // agregar un proyectil
    void update(const Screen &screen); // mover y eliminar inactivos
    void draw() const;                 // dibujar proyectiles
    std::vector<Projectile>& getProyectiles() { return proyectiles; }
};

#endif
