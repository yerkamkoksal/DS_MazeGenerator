# Maze Generation and Path Discovery Project

## Overview
This project comprises two phases: Maze Generation and Path Discovery. Implemented in C++, it first generates mazes based on user input and then discovers paths from an entry to an exit point within a specified maze.

## Features
- **Phase 1: Maze Generation**
  - Generates multiple mazes based on user inputs for dimensions (M x N) and the number of mazes (K).
  - Uses a stack-based approach to knock down walls randomly and create a maze.
  - Saves each generated maze in a file named "maze_mazeID.txt".

- **Phase 2: Path Discovery**
  - Loads a specified maze and attempts to find a path from a given entry to an exit point.
  - Saves the discovered path to a file named "maze_mazeID_path_entryX=_entryY_exitX_exitY.txt".
  - Handles cases where no path is found.

## Compilation and Execution
Ensure you have a C++ compiler installed (e.g., g++, clang).
1. Compile the code:
   ```bash
   g++ [filename].cpp -o MazeProject
2. Run the executable:
   ./MazeProject

## Usage

The program will prompt for input values for both phases.
For maze generation, enter the number of mazes (K) and their dimensions (M rows, N columns).
For path discovery, provide the mazeID, entry (X, Y) coordinates, and exit (X, Y) coordinates.
Contributing

Suggestions and improvements are welcome. Feel free to fork the project, open issues, or send pull requests.

## Contact

Yusuf Erkam Köksal
Email: yusuferkamkoksal@outlook.com
GitHub: yerkamkoksal

## License

This project is open source and available under the MIT License.

