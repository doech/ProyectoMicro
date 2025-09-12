# 🚀 Proyecto Asteroids (CC3086 – Programación de Microprocesadores)

Este proyecto es una implementación del clásico juego **Asteroids** en **C++** utilizando **hilos POSIX (pthread)** y la librería **ncurses** para la interfaz gráfica en consola.  
Forma parte del Proyecto 1 del curso **CC3086 Programación de Microprocesadores – Universidad del Valle de Guatemala**.

---

## 📂 Estructura del proyecto

```
ProyectoMicro/
├── Main.cpp        # Punto de entrada del juego (menú, tablero, instrucciones)
├── scores.hpp      # Declaraciones de la clase HighScores (API de puntajes)
├── scores.cpp      # Implementación de HighScores (lectura/escritura CSV + render en ncurses)
├── scores.csv      # Archivo con puntajes persistentes (nombre;puntaje;timestamp)
└── README.md       # Este archivo
```

### Archivos principales
- **Main.cpp**  
  - Menú principal (Iniciar partida, Instrucciones, Puntajes, Salir).  
  - Tablero de juego (ASCII-Art con ncurses).  
  - Conexión con el sistema de puntajes.  

- **scores.hpp / scores.cpp**  
  - Manejan la lógica de la **tabla de puntajes destacados**.  
  - Guardan y cargan datos desde `scores.csv`.  
  - Muestran el ranking en una tabla con ncurses.  

- **scores.csv**  
  - Archivo de texto plano con formato:  
    ```
    nombre;puntaje;timestamp
    ```
  - Ejemplo:  
    ```
    Esteban;150;1736737200
    Alejandro;120;1736650800
    ```

---

## ⚙️ Requisitos

Para compilar y ejecutar este proyecto necesitas:
- Linux (o **WSL en Windows**).
- Paquetes de desarrollo:
  ```bash
  sudo apt update
  sudo apt install build-essential libncurses5-dev libncursesw5-dev -y
  ```

---

## ▶️ Compilación y ejecución

Desde la carpeta del proyecto, ejecutar:

```bash
g++ -std=c++17 Main.cpp scores.cpp -lncurses -o asteroids
./asteroids
```

---

## 🎮 Controles del juego (en desarrollo)

- **Jugador 1:**  
  - `WASD` → mover nave.  
  - `Espacio` → disparar.  

- **Jugador 2:**  
  - Flechas → mover nave.  
  - `Enter` → disparar.  

---

## 📊 Sistema de puntajes destacados

- Se almacena automáticamente en `scores.csv`.  
- Cada entrada incluye:  
  - **Nombre del jugador**.  
  - **Puntos finales**.  
  - **Fecha (timestamp UNIX)**.  
- La tabla de puntajes muestra el **Top 10 ordenado por puntos**.  
- Se accede desde el menú principal con la opción **“Puntajes destacados”**.  

Ejemplo de visualización en consola:

```
===== PUNTAJES DESTACADOS =====

#  Jugador         Puntos   Fecha
-----------------------------------
1. Diego           200      2025-01-10
2. Esteban         150      2025-01-12
3. Alejandro       120      2025-01-11
4. Alejandra        95      2025-01-10
```

---

## 📌 Próximos pasos
- Implementar lógica completa del **juego concurrente** con pthreads.  
- Añadir pantalla de **Game Over** con registro automático de puntajes.  
- Permitir ingreso del **nombre del jugador** en ncurses al finalizar la partida.  

---

## 👨‍💻 Autores
- Esteban De la Peña  
- Alejandro Pérez  
- Alejandra Sierra  
- Diego Quan  

Universidad del Valle de Guatemala – Ciclo 2, 2025
