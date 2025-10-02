#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <ncurses.h>
#include <vector>
#include "Screen.hpp"

class Player {
private:
    int x, y;
    int vidas;
    int invulnerableFrames;

public:
    Player(int startX, int startY);

    void mover(int tecla, const Screen &screen);
    void dibujar() const;

    int getX() const { return x; }
    int getY() const { return y; }
    int getVidas() const { return vidas; }

    void perderVida() {
        if (invulnerableFrames == 0 && vidas > 0) {
            vidas--;
            invulnerableFrames = 50; // medio segundo si cada frame ~25ms
        }
    }

    void update() {
        if (invulnerableFrames > 0) invulnerableFrames--;
    }
    bool estaVivo() const { return vidas > 0; }
    bool estaInvulnerable() const { return invulnerableFrames > 0; }
};

#endif
