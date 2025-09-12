#pragma once
#include <string>
#include <vector>

struct Score {
    std::string name;
    int points;
    long ts; // epoch seconds
};

class HighScores {
public:
    explicit HighScores(const std::string& filePath = "scores.csv", size_t maxKeep = 10);

    // Carga de disco (si no existe, queda vacío)
    bool load();
    // Guarda a disco (crea/overwrites)
    bool save() const;

    // Agrega y mantiene orden (desc) y tamaño
    void add(const std::string& name, int points, long ts);

    // Devuelve copia del Top N (<= maxKeep)
    std::vector<Score> top(size_t n) const;

    // Pantalla ncurses: muestra la tabla y espera tecla
    void renderNcurses();

private:
    std::string path;
    size_t maxKeep;
    std::vector<Score> data;

    void sortAndTrim();
};
