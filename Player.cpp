#include "Player.hpp"
#include <unistd.h>

int Player::nextId = 0;

void Player::mover()
{
    switch (direccion.load())
    {
    case UP:
        if (y > screen->top + 1)
            y--;
        break;
    case DOWN:
        if (y < screen->bottom - 1)
            y++;
        break;
    case LEFT:
        if (x > screen->left + 1)
            x--;
        break;
    case RIGHT:
        if (x < screen->right - 1)
            x++;
        break;
    case NONE: /* no hacer nada */
        break;
    }
}

void Player::draw() const
{
    if (invulnerableFrames.load() > 0)
    {
        // parpadeo: visible un frame, invisible el siguiente
        if (invulnerableFrames.load() % 4 == 0)
        {
            mvprintw(y, x, "^");
        }
    }
    else
    {
        mvprintw(y, x, "^");
    }
}

void *Player::ciclo(void *arg)
{
    Player *p = (Player *)arg;

    while (p->vidas.load() > 0)
    {
        p->mover();
        usleep(50000);
    }

    return nullptr;
}

//---------Manager---------

void PlayerManager::spawn(double x, double y, Controls c)
{
    auto p = std::make_unique<Player>(x, y, c, *screen, *projectileManager);
    p->iniciar(); // lanza el hilo
    players.push_back(std::move(p));
}

void PlayerManager::update()
{
    for (auto &p : players) {
        if (p->estaVivo()) {
            p->update();
        }
    }
}

void PlayerManager::draw() const
{
    for (const auto &p : players)
    {
        p->draw();
    }
}