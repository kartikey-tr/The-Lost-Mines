# 🏰 The Lost Mines

**The Lost Mines** is a console-based adventure game written in C. It features procedural level generation, a dynamic lighting system, and turn-based tactical combat.

---

## 🎮 Game Description

You are the Omega Warrior (`Ω`). You have been dropped into a pitch-black dungeon inhabited by the Psi Demons (`Ψ`). Your mission is to explore the darkness, collect the ancient Diamonds (`♦`), and escape through the Gate (`∩`) before your health runs out.

Each time you play, the map is **procedurally generated**, meaning no two dungeons are ever the same.

## ✨ Key Features

* **🔦 Dynamic Fog of War:** The map is hidden in darkness. Areas are revealed only as you explore them, and visited areas remain in your memory (faded grey).
* **⛏️ Procedural Generation:** Uses a "Drunkard's Walk" algorithm to dig unique, organic cave systems every game.
* **⚔️ Turn-Based Combat:** Enemies move only when you move. Strategy is key—don't get surrounded!
* **🧠 Smart Pathing:** Enemies use Manhattan distance logic to chase the player without unfair diagonal jumps.
* **💻 Pure C:** Built using standard C libraries and Windows API for console manipulation.

## 🕹️ Controls

The game supports both **WASD** and **Arrow Keys**.

| Action | Key(s) |
| :--- | :--- |
| **Move Up** | `W` or `↑` |
| **Move Left** | `A` or `←` |
| **Move Down** | `S` or `↓` |
| **Move Right** | `D` or `→` |
| **Quit Game** | `Esc` (at end screen) |

## 🗺️ Legend / Symbols

* `Ω` **(Cyan):** Player (You)
* `Ψ` **(Red):** Enemy (Psi Demon)
* `♦` **(Yellow):** Diamond (Objective)
* `♥` **(Pink):** Health Potion (+30 HP)
* `∩` **(Green):** The Exit Gate
* `▓` **(Grey):** Wall
* `░` **(Dark Grey):** Explored Area (Memory)

## 🚀 How to Compile & Run

You can compile this code using any standard C compiler (like GCC).

**Using GCC:**
```bash
gcc dungeon_omega.c -o dungeon_omega.exe
./dungeon_omega.exe
