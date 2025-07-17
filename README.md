# Back-Tracking Algorithms – AI vs Human Board Game

This project is a strategy-based 2-player board game implemented in C++, where a human competes against an AI opponent using backtracking algorithms.

## 🎮 Game Overview

- The game is played on a **5×5 grid board**.
- Both players (human and AI) start on **Adjacent edges** (ignoring corners).
- Each player controls **3 moving pieces**.
- On each turn, a player can:
  - Move **1 block** in a valid direction.
  - **Jump over one enemy piece** (like in checkers), but only **one piece at a time**.
- The goal is to **move all of your pieces to the opposite side** before your opponent does.

## 🤖 AI Behavior

- The AI uses **backtracking** to evaluate all valid move sequences and chooses optimal strategies.
- It can skip over one enemy block if allowed.
- Designed to provide a competitive challenge to human players.

## 🛠️ Built With

- **Language**: C++
- **Paradigm**: Object-Oriented Programming (OOP)
- **Core Algorithm**: Backtracking
- **Development Tools**: Any C++ IDE or g++ compiler

## 🚀 How to Compile and Run

### Using g++ (Linux/macOS/WSL):
```bash
g++ -o aoa_game main.cpp
./aoa_game

