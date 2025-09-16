// ========== ncurses ==========
#include <ncurses.h>
#include "scores.hpp"
#include <algorithm>
#include <ctime>
#include <fstream>
#include <sstream>
#include <cstring> 

// --- helpers ---
static std::string trim(const std::string& s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    size_t b = s.find_last_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    return s.substr(a, b - a + 1);
}

// "Constructor/Instanciador", recibe el path del archivo y la cantidad maxima de jugadores en la tabla
HighScores::HighScores(const std::string& filePath, size_t maxKeep_)
: path(filePath), maxKeep(maxKeep_) {}


bool HighScores::load() {
    data.clear();
    std::ifstream in(path);
    if (!in.is_open()) return false; // primera vez: no pasa nada

    std::string line;
    while (std::getline(in, line)) {
        line = trim(line);
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string name, pointsStr, tsStr;

        if (!std::getline(ss, name, ';')) continue;
        if (!std::getline(ss, pointsStr, ';')) continue;
        if (!std::getline(ss, tsStr, ';')) tsStr = "0";

        Score s;
        s.name = trim(name);
        try {
            s.points = std::stoi(pointsStr);
            s.ts = std::stol(tsStr);
        } catch (...) {
            continue;
        }
        data.push_back(s);
    }
    sortAndTrim();
    return true;
}

bool HighScores::save() const {
    std::ofstream out(path, std::ios::trunc);
    if (!out.is_open()) return false;
    for (const auto& s : data) {
        out << s.name << ';' << s.points << ';' << s.ts << '\n';
    }
    return true;
}

void HighScores::add(const std::string& name, int points, long ts) {
    data.push_back(Score{name, points, ts});
    sortAndTrim();
}

std::vector<Score> HighScores::top(size_t n) const {
    if (n > data.size()) n = data.size();
    return std::vector<Score>(data.begin(), data.begin() + n);
}

void HighScores::sortAndTrim() {
    std::sort(data.begin(), data.end(),
        [](const Score& a, const Score& b){
            if (a.points != b.points) return a.points > b.points;  // desc por puntaje
            return a.ts < b.ts; // en empate: más antiguo primero (o cambia a > si prefieres reciente)
        });
    if (data.size() > maxKeep) data.resize(maxKeep);
}



static std::string formatDate(long ts) {
    if (ts <= 0) return "-";
    std::time_t t = static_cast<std::time_t>(ts);
    std::tm* tm = std::localtime(&t);
    char buf[32];
    if (tm && std::strftime(buf, sizeof(buf), "%Y-%m-%d", tm)) return buf;
    return "-";
}

void HighScores::renderNcurses() {
    clear();
    int max_y, max_x;
    getmaxyx(stdscr, max_y, max_x);

    const char* title = "===== PUNTAJES DESTACADOS =====";
    mvprintw(2, (max_x - (int)strlen(title))/2, "%s", title);

    // Encabezado
    int startY = 5;
    mvprintw(startY,   6, "#");
    mvprintw(startY,  10, "Jugador");
    mvprintw(startY,  40, "Puntos");
    mvprintw(startY,  52, "Fecha");

    // Línea separadora
    for (int x = 4; x < max_x-4; ++x) mvaddch(startY+1, x, '-');

    // Filas
    auto top10 = top(10);
    int row = startY + 3;
    int rank = 1;
    for (const auto& s : top10) {
        mvprintw(row,   6, "%2d", rank++);
        mvprintw(row,  10, "%.26s", s.name.c_str());
        mvprintw(row,  40, "%6d", s.points);
        mvprintw(row,  52, "%s", formatDate(s.ts).c_str());
        row++;
    }
    if (top10.empty()) {
        mvprintw(row, 10, "No hay puntajes aun. Juega una partida!");
        row++;
    }

    mvprintw(max_y - 2, 6, "Presione cualquier tecla para volver al menu...");
    refresh();
    getch();
}
