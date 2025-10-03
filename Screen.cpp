#include "Screen.hpp"
#include <list>
#include <memory>
#include "Player.hpp"

Screen::Screen()
{
    getmaxyx(stdscr, max_y, max_x);

    // Definir límites jugables
    top = 3;
    bottom = max_y - 3;
    left = 1;
    right = max_x - 2;
}

void Screen::clearScreen()
{
    clear();
}

void Screen::refreshScreen()
{
    refresh();
}

void Screen::drawHUD(const std::list<std::unique_ptr<Player>> &players, int modo)
{
    // Barra superior
    int modo_space = 12;                        // ancho reservado para mostrar "Modo: X"
    int players_space = max_x - modo_space - 2; // resto del ancho para jugadores (dejamos un margen)
    int n = players.size();
    int section = (n > 0) ? players_space / n : players_space; // espacio por jugador

    // Dibujar info de jugadores
    int idx = 0;
    for (auto &p : players)
    {
        int pos_x = 2 + idx * section; // posición de inicio de la sección

        // Puntaje del jugador
        mvprintw(1, pos_x, "Player %d Score:%03d", idx + 1, p->getPuntaje());

        // Vidas del jugador (debajo del score, en su misma sección)
        mvprintw(2, pos_x, "Vidas:");
        for (int i = 0; i < p->getVidas(); i++)
        {
            mvaddch(2, pos_x + 7 + i * 2, ACS_DIAMOND);
        }

        idx++;
    }

    // Dibujar el modo a la derecha
    mvprintw(1, max_x - modo_space, "Modo: %d", modo);

    // Línea divisoria (más abajo, para separar HUD del juego)
    for (int x = 0; x < max_x; x++)
    {
        mvaddch(3, x, '-');
    }
}


void Screen::drawBorders()
{
    // Bordes izquierdo y derecho
    for (int y = top; y < bottom; y++)
    {
        mvprintw(y, 0, "|");
        mvprintw(y, max_x - 1, "|");
    }

    // Borde inferior
    for (int x = 0; x < max_x; x++)
    {
        mvprintw(bottom, x, "-");
    }
}

void Screen::drawMessage(const std::string &msg)
{
    mvprintw(max_y - 2, 2, "%s", msg.c_str());
}

void Screen::drawGameOver(int puntaje, std::string &nombreJugador)
{
    clear();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    std::string msg = "GAME OVER";
    std::string score = "Puntaje final: " + std::to_string(puntaje);

    mvprintw(max_y / 2 - 2, (max_x - msg.size()) / 2, "%s", msg.c_str());
    mvprintw(max_y / 2, (max_x - score.size()) / 2, "%s", score.c_str());

    mvprintw(max_y / 2 + 2, (max_x - 20) / 2, "Ingrese su nombre: ");
    echo(); // permite mostrar lo que se escribe
    char buffer[50];
    getnstr(buffer, 49); // leer hasta 49 caracteres
    noecho();
    nombreJugador = buffer;

    refresh();
}
