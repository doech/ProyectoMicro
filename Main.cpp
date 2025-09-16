#include <ncurses.h> //Manejo de pantalla y entradas de consola
#include <iostream>
#include <locale.h> // Necesario para Unicode
#include "scores.hpp"
#include <ctime>   // std::time
HighScores HS("scores.csv", 10);

int mostrarMenu() {
    int opcion = 0;
    while (true) {
        clear(); // Limpia la pantalla en cada iteración

        mvprintw(2, 5, "===== ASTEROIDS =====");
        mvprintw(4, 5, "1. Iniciar partida");
        mvprintw(5, 5, "2. Ver instrucciones");
        mvprintw(6, 5, "3. Puntajes destacados");
        mvprintw(7, 5, "4. Salir");
        mvprintw(9, 5, "Seleccione una opcion: ");

        refresh(); // Actualiza la pantalla

        int tecla = getch();
        if (tecla == '1') { opcion = 1; break; }
        if (tecla == '2') { opcion = 2; break; }
        if (tecla == '3') { opcion = 3; break; }
        if (tecla == '4') { opcion = 4; break; }
    }
    return opcion;
}

void mostrarInstrucciones() {
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

void mostrarTablero() {
    clear();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    // ===== ZONA SUPERIOR (HUD) =====
    mvprintw(0, 0, "========================================");
    mvprintw(1, 2, "Vidas: ");
    mvaddch(1, 10, ACS_DIAMOND);
    mvaddch(1, 12, ACS_DIAMOND);
    mvaddch(1, 14, ACS_DIAMOND);

    mvprintw(1, max_x/2 - 7, "Puntaje: 000");
    mvprintw(1, max_x - 12, "Modo: 1");

    // Línea divisoria
    for (int x = 0; x < max_x; x++) {
        mvprintw(2, x, "-");
    }

    // ===== CAMPO DE JUEGO =====
    // Borde izquierdo y derecho
    for (int y = 3; y < max_y - 3; y++) {
        mvprintw(y, 0, "|");
        mvprintw(y, max_x - 1, "|");
    }

    // Borde inferior del campo
    for (int x = 0; x < max_x; x++) {
        mvprintw(max_y - 3, x, "-");
    }

    // Elementos de prueba
    mvprintw(max_y/2, max_x/4, "^");     // nave
    mvprintw(max_y/3, max_x/3, "O");     // asteroide grande
    mvprintw(max_y/4, max_x/2, "o");     // asteroide pequeño
    mvprintw(max_y/2 - 2, max_x/4, "|"); // proyectil

    // ===== ZONA INFERIOR (mensajes) =====
    mvprintw(max_y - 2, 0, "Presione cualquier tecla para volver al menu");

    mvprintw(max_y - 1, 0, "========================================");

    refresh();
    getch(); // espera tecla para volver
}



int main() {

    setlocale(LC_ALL, ""); // Activa soporte para UTF-8 en ncurses

    // Inicializar ncurses
    initscr();            // Inicia el modo ncurses
    HS.load();
    cbreak();             // Captura inmediata de teclas (sin esperar Enter)
    noecho();             // No mostrar las teclas que el usuario presiona
    keypad(stdscr, TRUE); // Habilita teclas especiales (flechas, F1…)
    curs_set(0);          // Oculta el cursor

    int opcion;
    do {
        opcion = mostrarMenu();
        if (opcion == 1) {
            mostrarTablero();
        } else if (opcion == 2) {
            mostrarInstrucciones();
        }
        else if (opcion == 3) {
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

// Registra un puntaje con timestamp actual y persiste en disco
void registrarPuntaje(const std::string& nombreJugador, int puntosFinales) {
    long ts = std::time(nullptr);   // epoch seconds (fecha/hora actuales)
    HS.add(nombreJugador, puntosFinales, ts);
    HS.save();                      // persiste en scores.csv
}