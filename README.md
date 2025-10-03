# 🚀 Proyecto Asteroids (CC3086 – Programación de Microprocesadores)

Este proyecto es una implementación del clásico juego **Asteroids** en **C++** utilizando la librería **ncurses** para la interfaz gráfica en consola.  
Forma parte del Proyecto 1 del curso **CC3086 Programación de Microprocesadores – Universidad del Valle de Guatemala**.

Actualmente se encuentra **funcional** con el motor principal, colisiones, sistema de puntajes y HUD, y pendiente de migrar la lógica a **pthread** y añadir **niveles** de dificultad.

---

## 📂 Estructura del proyecto

```
ProyectoMicro/
├── Main.cpp              # Punto de entrada del juego
├── Game.cpp/.hpp         # Clase Game: bucle principal, input, actualización y renderizado
├── Player.cpp/.hpp       # Clase Player: nave, movimiento, vidas, daño
├── Projectile.cpp/.hpp   # Clase Projectile y ProjectileManager
├── Asteroid.cpp/.hpp     # Clase Asteroid y AsteroidManager
├── Screen.cpp/.hpp       # Clase Screen: HUD, tablero, pantalla de Game Over
├── Input.cpp/.hpp        # Manejo de entradas de teclado
├── scores.cpp/.hpp       # HighScores: lectura/escritura CSV
├── scores.csv            # Archivo con puntajes persistentes (nombre;puntaje;timestamp)
└── README.md             # Este archivo
```

---

## 🎮 Características implementadas

- **Jugador (Player)**
  - Movimiento con `WASD`.
  - Disparo con barra espaciadora.
  - Sistema de vidas y parpadeo al recibir daño.

- **Proyectiles (ProjectileManager)**
  - Se eliminan al salir de la pantalla.
  - Colisionan con asteroides y los destruyen.

- **Asteroides (AsteroidManager)**
  - Se mueven con incrementos `dx, dy` (`double`) para trayectorias suaves.
  - Rebotan en los bordes laterales.
  - Respawnean desde arriba si llegan al fondo.
  - Colisión con proyectiles → destruidos con animación.
  - Colisión con jugador → resta vidas.

- **Pantalla y HUD (Screen)**
  - Tablero con límites.
  - HUD superior (vidas, puntaje, modo).
  - Mensajes en zona inferior.
  - Pantalla de **Game Over**.

- **Sistema de Puntajes (HighScores)**
  - Registro de nombre y puntaje final.
  - Guardado automático en `scores.csv`.
  - Persistencia y lectura desde el menú.

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

Desde la carpeta del proyecto:

```bash
g++ -std=c++17 Main.cpp Game.cpp Player.cpp Input.cpp Screen.cpp Projectile.cpp Asteroid.cpp scores.cpp -lncurses -o asteroids
./asteroids
```

---

## 📊 Sistema de puntajes

Archivo `scores.csv` con formato:

```
nombre;puntaje;timestamp
```

El Top 10 se muestra desde el menú principal.

---

## 👨‍💻 Autores

- Esteban De la Peña  
- Alejandro Pérez  
- Alejandra Sierra  
- Diego Quan  

**Universidad del Valle de Guatemala – Ciclo 2, 2025**
