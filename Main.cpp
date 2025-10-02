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
        clear(); // Limpia la pantalla en cada iteración

        mvprintw(2, 5, "===== ASTEROIDS =====");
        mvprintw(4, 5, "1. Iniciar partida");
        mvprintw(5, 5, "2. Ver instrucciones");
        mvprintw(6, 5, "3. Puntajes destacados");
        mvprintw(7, 5, "4. Salir");
        mvprintw(9, 5, "Seleccione una opcion: ");

        refresh(); // Actualiza la pantalla

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
    mvprintw(2, 5, "===== INSTRUCCIONES =====");
    mvprintw(4, 5, "Objetivo: Destruir asteroides y sobrevivir con 3 vidas.");
    mvprintw(6, 5, "Controles:");
    mvprintw(7, 7, "Jugador 1 -> WASD para moverse, ESPACIO para disparar.");
    mvprintw(8, 7, "Jugador 2 -> Flechas para moverse, ENTER para disparar.");
    mvprintw(10, 5, "Condiciones de victoria:");
    mvprintw(11, 7, "Modo 1: Ganas si destruyes 6 asteroides pequenos.");
    mvprintw(12, 7, "Modo 2: Ganas si destruyes 10 asteroides pequenos.");
    mvprintw(13, 7, "En cualquier modo pierdes si pierdes tus 3 vidas.");

    mvprintw(15, 5, "Presione cualquier tecla para volver al menu...");
    refresh();
    getch(); // Espera que el usuario presione algo para volver
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
            Game game;
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
            mvprintw(2, 5, "===== PUNTAJES DESTACADOS =====");
            /* mvprintw(4, 5, "(Esta seccion se implementara mas adelante)");
            mvprintw(6, 5, "Presione cualquier tecla para volver al menu..."); */
            HS.renderNcurses();
            refresh();
            getch();
        }
    } while (opcion != 4); // Se repite hasta que elija salir

    // Restaurar la terminal
    endwin();
    return 0;
}

