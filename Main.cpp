#include <ncurses.h> // Manejo de pantalla y entradas de consola
#include <locale.h>  // Necesario para Unicode
#include "scores.hpp"
#include "Game.hpp"
#include <ctime>    // std::time
#include <string.h> // strlen

HighScores HS("scores.csv", 10);

int mostrarMenu()
{
    const char *opciones[] = {
        "INICIAR PARTIDA",
        "INSTRUCCIONES",
        "PUNTAJES ALTOS",
        "SALIR"};
    int n = 4;
    int seleccion = 0;

    while (true)
    {
        clear();
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        int startY = (maxY - 25) / 2;
        int centerX = maxX / 2;

        // Título ASCII
        mvprintw(startY + 2, centerX - 32, "   ###     ####  ######  #####  #####    ####   ####  ####    #### ");
        mvprintw(startY + 3, centerX - 32, "  ## ##   ##       ##    ##     ##  ##  ##  ##   ##   ##  ##  ##   ");
        mvprintw(startY + 4, centerX - 32, " ##   ##   ###     ##    ####   #####   ##  ##   ##   ##  ##   ### ");
        mvprintw(startY + 5, centerX - 32, " #######     ##    ##    ##     ##  ##  ##  ##   ##   ##  ##     ##");
        mvprintw(startY + 6, centerX - 32, " ##   ##  ####     ##    #####  ##  ##   ####   ####  ####    #### ");

        mvprintw(startY + 9, centerX - 17, "=========== MENU PRINCIPAL ===========");

        // Opciones
        for (int i = 0; i < n; i++)
        {
            if (i == seleccion)
                attron(A_REVERSE);
            mvprintw(startY + 12 + i * 2, centerX - (strlen(opciones[i]) / 2), "[ %s ]", opciones[i]);
            if (i == seleccion)
                attroff(A_REVERSE);
        }

        refresh();

        int tecla = getch();
        if (tecla == KEY_UP || tecla == 'w')
            seleccion = (seleccion - 1 + n) % n;
        else if (tecla == KEY_DOWN || tecla == 's')
            seleccion = (seleccion + 1) % n;
        else if (tecla == '\n' || tecla == ' ')
            return seleccion + 1; // devuelve 1-4
    }
}

void mostrarInstrucciones()
{
    clear();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    int startY = (maxY - 22) / 2;
    int centerX = maxX / 2;

    // Marco
    for (int i = 0; i < maxX; i++)
    {
        mvprintw(startY, i, "=");
        mvprintw(startY + 20, i, "=");
    }

    mvprintw(startY + 2, centerX - 15, "================================");
    mvprintw(startY + 3, centerX - 15, "     INSTRUCCIONES DE JUEGO    ");
    mvprintw(startY + 4, centerX - 15, "================================");

    mvprintw(startY + 6, centerX - 30, "OBJETIVO: Destruir asteroides y sobrevivir con 3 vidas");

    mvprintw(startY + 8, centerX - 30, "+---------------------------------------------------------+");
    mvprintw(startY + 9, centerX - 30, "|  CONTROLES:                                            |");
    mvprintw(startY + 10, centerX - 30, "|                                                        |");
    mvprintw(startY + 11, centerX - 30, "|  JUGADOR 1:  [W][A][S][D] Movimiento                  |");
    mvprintw(startY + 12, centerX - 30, "|              [ESPACIO] Disparar                        |");
    mvprintw(startY + 13, centerX - 30, "|                                                        |");
    mvprintw(startY + 14, centerX - 30, "|  JUGADOR 2:  [FLECHAS] Movimiento                     |");
    mvprintw(startY + 15, centerX - 30, "|              [ENTER] Disparar                          |");
    mvprintw(startY + 16, centerX - 30, "+---------------------------------------------------------+");

    mvprintw(startY + 18, centerX - 25, ">> Presione cualquier tecla para continuar <<");

    refresh();
    getch();
}

int seleccionarModo()
{
    const char *modos[] = {
        "MODO 1 - Un jugador | Velocidad MEDIA | 6 asteroides",
        "MODO 2 - Un jugador | Velocidad ALTA | 10 asteroides",
        "MODO 3 - Dos jugadores | COMPETITIVO | Mas asteroides gana"};
    int n = 3;
    int seleccion = 0;

    while (true)
    {
        clear();
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        int startY = (maxY - 20) / 2;
        int centerX = maxX / 2;

        mvprintw(startY + 2, centerX - 20, "========================================");
        mvprintw(startY + 3, centerX - 20, "       SELECCIONAR MODO DE JUEGO       ");
        mvprintw(startY + 4, centerX - 20, "========================================");

        for (int i = 0; i < n; i++)
        {
            if (i == seleccion)
                attron(A_REVERSE);
            mvprintw(startY + 7 + i * 2, centerX - (strlen(modos[i]) / 2), "%s", modos[i]);
            if (i == seleccion)
                attroff(A_REVERSE);
        }

        mvprintw(startY + 16, centerX - 25, ">> Presione ESC o cualquier tecla no valida para volver <<");

        refresh();

        int tecla = getch();
        if (tecla == KEY_UP || tecla == 'w')
            seleccion = (seleccion - 1 + n) % n;
        else if (tecla == KEY_DOWN || tecla == 's')
            seleccion = (seleccion + 1) % n;
        else if (tecla == '\n' || tecla == ' ')
            return seleccion + 1; // devuelve 1-3
        else
            return 0; // cualquier otra tecla → regresar al menu principal
    }
}

int main()
{
    setlocale(LC_ALL, "en_US.UTF-8"); // Activa soporte para UTF-8 en ncurses

    // Inicializar ncurses
    initscr();
    HS.load();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int opcion;
    do
    {
        opcion = mostrarMenu();
        if (opcion == 1)
        {
            int modo = seleccionarModo();
            if (modo != 0) // solo corre el juego si eligió un modo válido
            {
                Game game(modo, HS);
                game.run();
                
            }
        }
        else if (opcion == 2)
        {
            mostrarInstrucciones();
        }
        else if (opcion == 3)
        {
            clear();
            int maxY, maxX;
            getmaxyx(stdscr, maxY, maxX);
            int centerX = maxX / 2;

            mvprintw(2, centerX - 20, "========================================");
            mvprintw(3, centerX - 20, "       PUNTAJES DESTACADOS             ");
            mvprintw(4, centerX - 20, "========================================");

            HS.renderNcurses();

            refresh();
            //getch();
        }
    } while (opcion != 4);

    endwin();
    return 0;
}
