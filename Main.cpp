#include <ncurses.h> //Manejo de pantalla y entradas de consola
#include <locale.h>  // Necesario para Unicode
#include "scores.hpp"
#include "Game.hpp"
#include <ctime> // std::time
HighScores HS("scores.csv", 10);

// Registra un puntaje con timestamp actual y persiste en disco
void registrarPuntaje(const std::string &nombreJugador, int puntosFinales)
{
    long ts = std::time(nullptr); // epoch seconds (fecha/hora actuales)
    HS.add(nombreJugador, puntosFinales, ts);
    HS.save(); // persiste en scores.csv
}

int mostrarMenu()
{
    int opcion = 0;
    while (true)
    {
        clear();
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        
        int startY = (maxY - 25) / 2;
        int centerX = maxX / 2;
        
        // Marco superior
        for(int i = 0; i < maxX; i++) {
            mvprintw(startY, i, "=");
            mvprintw(startY + 23, i, "=");
        }
        
        // Titulo grande ASCII art
        mvprintw(startY + 2, centerX - 32, "   ###     ####  ######  #####  #####    ####   ####  ####    #### ");
        mvprintw(startY + 3, centerX - 32, "  ## ##   ##       ##    ##     ##  ##  ##  ##   ##   ##  ##  ##   ");
        mvprintw(startY + 4, centerX - 32, " ##   ##   ###     ##    ####   #####   ##  ##   ##   ##  ##   ### ");
        mvprintw(startY + 5, centerX - 32, " #######     ##    ##    ##     ##  ##  ##  ##   ##   ##  ##     ##");
        mvprintw(startY + 6, centerX - 32, " ##   ##  ####     ##    #####  ##  ##   ####   ####  ####    #### ");
        
        // Decoracion estilo arcade
        mvprintw(startY + 8, centerX - 30, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
        mvprintw(startY + 9, centerX - 30, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
        
        // Menu centrado
        mvprintw(startY + 11, centerX - 15, "+---------------------------------+");
        mvprintw(startY + 12, centerX - 15, "|                                 |");
        mvprintw(startY + 13, centerX - 15, "|   [1] >> INICIAR PARTIDA <<     |");
        mvprintw(startY + 14, centerX - 15, "|                                 |");
        mvprintw(startY + 15, centerX - 15, "|   [2] >> INSTRUCCIONES <<       |");
        mvprintw(startY + 16, centerX - 15, "|                                 |");
        mvprintw(startY + 17, centerX - 15, "|   [3] >> PUNTAJES ALTOS <<      |");
        mvprintw(startY + 18, centerX - 15, "|                                 |");
        mvprintw(startY + 19, centerX - 15, "|   [4] >> SALIR <<               |");
        mvprintw(startY + 20, centerX - 15, "|                                 |");
        mvprintw(startY + 21, centerX - 15, "+---------------------------------+");

        refresh();

        int tecla = getch();
        if (tecla == '1')
        {
            opcion = 1;
            break;
        }
        if (tecla == '2')
        {
            opcion = 2;
            break;
        }
        if (tecla == '3')
        {
            opcion = 3;
            break;
        }
        if (tecla == '4')
        {
            opcion = 4;
            break;
        }
    }
    return opcion;
}

void mostrarInstrucciones()
{
    clear();
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);
    
    int startY = (maxY - 22) / 2;
    int centerX = maxX / 2;
    
    // Marco
    for(int i = 0; i < maxX; i++) {
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
    int modo = 0;
    while (true)
    {
        clear();
        int maxY, maxX;
        getmaxyx(stdscr, maxY, maxX);
        
        int startY = (maxY - 20) / 2;
        int centerX = maxX / 2;
        
        // Marco
        for(int i = 0; i < maxX; i++) {
            mvprintw(startY, i, "=");
            mvprintw(startY + 18, i, "=");
        }
        
        mvprintw(startY + 2, centerX - 20, "========================================");
        mvprintw(startY + 3, centerX - 20, "       SELECCIONAR MODO DE JUEGO       ");
        mvprintw(startY + 4, centerX - 20, "========================================");
        
        mvprintw(startY + 6, centerX - 35, "+--------------------------------------------------------------------+");
        mvprintw(startY + 7, centerX - 35, "|                                                                    |");
        mvprintw(startY + 8, centerX - 35, "|  [1] MODO 1 - Un jugador | Velocidad MEDIA | 6 asteroides        |");
        mvprintw(startY + 9, centerX - 35, "|                                                                    |");
        mvprintw(startY + 10, centerX - 35, "|  [2] MODO 2 - Un jugador | Velocidad ALTA | 10 asteroides        |");
        mvprintw(startY + 11, centerX - 35, "|                                                                    |");
        mvprintw(startY + 12, centerX - 35, "|  [3] MODO 3 - Dos jugadores | COMPETITIVO | Mas asteroides gana  |");
        mvprintw(startY + 13, centerX - 35, "|                                                                    |");
        mvprintw(startY + 14, centerX - 35, "+--------------------------------------------------------------------+");
        
        mvprintw(startY + 16, centerX - 20, ">> Seleccione un modo [1/2/3] <<");
        
        refresh();

        int tecla = getch();
        if (tecla == '1')
        {
            modo = 1;
            break;
        }
        if (tecla == '2')
        {
            modo = 2;
            break;
        }
        if (tecla == '3')
        {
            modo = 3;
            break;
        }
    }
    return modo;
}

int main()
{

    setlocale(LC_ALL, "en_US.UTF-8"); // Activa soporte para UTF-8 en ncurses

    // Inicializar ncurses
    initscr(); // Inicia el modo ncurses
    HS.load();
    cbreak();             // Captura inmediata de teclas (sin esperar Enter)
    noecho();             // No mostrar las teclas que el usuario presiona
    keypad(stdscr, TRUE); // Habilita teclas especiales (flechas, F1…)
    curs_set(0);          // Oculta el cursor

    int opcion;
    do
    {
        opcion = mostrarMenu();
        if (opcion == 1)
        {
            int modo = seleccionarModo(); // <<--- Nuevo menú
            Game game(modo);              // <<--- Le pasas el modo al juego
            game.run();
            registrarPuntaje(game.getNombreJugador(), game.getPuntajeFinal());
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
            
            mvprintw(maxY - 3, centerX - 25, ">> Presione cualquier tecla para volver <<");
            
            refresh();
            getch();
        }
    } while (opcion != 4); // Se repite hasta que elija salir

    // Restaurar la terminal
    endwin();
    return 0;
}
