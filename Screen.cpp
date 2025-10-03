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

void Screen::drawEndScreen(const std::list<std::unique_ptr<Player>> &players, bool win)
{
    nodelay(stdscr, FALSE);
    clear();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);
    int centerY = max_y / 2;
    int centerX = max_x / 2;

    // Mensaje principal
    std::string msg = win ? "YOU WIN!" : "GAME OVER";
    if (!win)
    {
        mvprintw(centerY - 7, centerX - 35, "  ####      ###      ##  ##    ######     ####   ##   ##  ######  ####");
        mvprintw(centerY - 6, centerX - 35, " ##  ##    ## ##    ## ## ##   ##        ##  ##  ##   ##  ##      ## ##   ");
        mvprintw(centerY - 5, centerX - 35, " ##       ##   ##   ## ## ##   ####      ##  ##  ##   ##  ####    ####");
        mvprintw(centerY - 4, centerX - 35, " ##  ###  #######  ##  ##  ##  ##        ##  ##   ## ##   ##      ## ##");
        mvprintw(centerY - 3, centerX - 35, "  #### #  ##   ##  ##      ##  ######     ####     ###    ######  ##  ##");
    }else{
        mvprintw(centerY - 7, centerX - 30, " ##  ##   ####   ##  ##    ##      ##  ####  ###   ##   ##");
        mvprintw(centerY - 6, centerX - 30, " ##  ##  ##  ##  ##  ##    ##      ##   ##  ## ##  ##   ##");
        mvprintw(centerY - 5, centerX - 30, " ##  ##  ##  ##  ##  ##     ## ## ##    ##  ## ##  ##   ##");
        mvprintw(centerY - 4, centerX - 30, "   ##    ##  ##  ##  ##     ## ## ##    ##  ##  ## ##     ");
        mvprintw(centerY - 3, centerX - 30, "   ##     ####    ####       ##  ##    #### ##   ###    ##");
    }

    /* mvprintw(max_y / 2 - 4, (max_x - msg.size()) / 2, "%s", msg.c_str()); */

    // Mostrar puntajes actuales
    int offset = 0;
    for (auto &p : players)
    {
        std::string score = "Player #" + std::to_string(offset + 1) +
                            " - Puntaje: " + std::to_string(p->getPuntaje());
        mvprintw(centerY + offset, (max_x - score.size()) / 2, "%s", score.c_str());
        offset++;
    }

    echo();
    curs_set(1); // Activar cursor visible/parpadeante

    char buffer[50];
    offset = 0;

    // Pedir nombre a cada jugador y guardarlo
    for (auto &p : players)
    {
        std::string prompt = "Ingrese nombre para Player #" + std::to_string(offset + 1) + ": ";
        mvprintw(centerY + 4 + offset, (max_x - 30) / 2, "%s", prompt.c_str());

        move(centerY + 4 + offset, (max_x - 30) / 2 + prompt.size());
        getnstr(buffer, 49);
        p->setNombre(buffer);

        offset++;
    }

    curs_set(0); // Ocultar cursor otra vez al terminar
    noecho();
    refresh();
}

bool Screen::confirmacionSalir()
{
    clear();

    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    std::string msg = "Seguro que quieres salir?";
    std::string opciones[2] = {"[ OK ]", "[ Cancelar ]"};
    int seleccion = 0; // 0 = OK, 1 = Cancelar

    // Loop para navegar con teclas
    int tecla;
    while (true)
    {
        clear();
        mvprintw(max_y / 2 - 2, (max_x - msg.size()) / 2, "%s", msg.c_str());

        for (int i = 0; i < 2; i++)
        {
            if (i == seleccion)
                attron(A_REVERSE); // resalta opción seleccionada

            mvprintw(max_y / 2 + i, (max_x - opciones[i].size()) / 2, "%s", opciones[i].c_str());

            if (i == seleccion)
                attroff(A_REVERSE);
        }

        refresh();

        tecla = getch();
        if (tecla == KEY_UP || tecla == 'w')
        {
            seleccion = (seleccion == 0) ? 1 : 0; // alterna
        }
        else if (tecla == KEY_DOWN || tecla == 's')
        {
            seleccion = (seleccion == 1) ? 0 : 1; // alterna
        }
        else if (tecla == '\n' || tecla == ' ') // Enter o espacio
        {
            return seleccion == 0; // true si OK
        }
    }
}
