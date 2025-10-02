#include "Player.hpp"

Player::Player(int startX, int startY) : x(startX), y(startY), vidas(3), invulnerableFrames(0)  {}

void Player::mover(int tecla, const Screen &screen) {
    if (tecla == 'w' && y > screen.top + 1) y--;
    if (tecla == 's' && y < screen.bottom - 1) y++;
    if (tecla == 'a' && x > screen.left + 1) x--;
    if (tecla == 'd' && x < screen.right - 1) x++;
}

void Player::dibujar() const {
    if (invulnerableFrames > 0) {
        // parpadeo: visible un frame, invisible el siguiente
        if (invulnerableFrames % 4 == 0) {
            mvprintw(y, x, "^");
        }
    } else {
        mvprintw(y, x, "^");
    }
}