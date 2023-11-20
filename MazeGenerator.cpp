// Phase 1: Maze Generation
// Ask user for input values K, M, and N
// Loop K times:
//     Create empty maze with M rows and N columns
//     Push (0,0) cell onto stack
//     While stack is not empty:
//         Pop top cell from stack
//         Choose a random wall of current cell to knock down
//         If neighbor on other side of wall is unvisited:
//             Add neighbor to maze and push onto stack
//     Save maze to file "maze_mazeID.txt" in specified format

// Phase 2: Path Discovery
// Ask user for input values mazeID, entryX, entryY, exitX, and exitY
// Load maze from file "maze_mazeID.txt"
// Create empty stack
// Push entry cell onto stack
// While stack is not empty:
//     Pop top cell from stack
//     If current cell is the exit cell:
//         Generate path by tracing back through stack
//         Save path to file "maze_mazeID_path_entryX=_entryY_exitX_exitY.txt"
//         Exit loop
//     Mark current cell as visited
//     For each unvisited neighbor cell:
//         Push neighbor onto stack
// If loop is exited without finding exit cell, print "No path found"


#include <unistd.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <stack>
#include <ostream>
#include <random>
#include <sstream>
using namespace std;

// Define a struct to store maze cell information
struct MazeCell {
    int x;
    int y;
    bool leftWall;
    bool rightWall;
    bool upWall;
    bool downWall;
    bool visited;
    MazeCell* next;
};

// Define a template-based stack using a linked list
template <typename T>
class Stack {
    private:
        struct Node {
            T data;
            Node* next;
            Node* prev;
            Node(T  value) : data(value), next(nullptr), prev(nullptr) {}
        };
        Node* top;
        int size;

    public:
        Stack() : top(nullptr), size(0) {}
        ~Stack() {
            Node* current = top;
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
        }

        void push(T value) {
            Node* node = new Node(value);
            node->next = top;
            top = node;
            size++;
        }

        T pop() {
            if (size == 0) {
                throw runtime_error("Stack is empty");
            }
            Node* node = top;
            T value = node->data;
            top = node->next;
            delete node;
            size--;
            return value;
        }

        bool empty() const {
            return size == 0;
        }
    Node* getTop(){
        return this->top;
    }
    T peek() const {
            if (top == nullptr) {
                throw out_of_range("Stack is empty");
            }
            return top->data;
        }
};

template <typename T>
void mazeGenerator(int M, int N, int K) {
    // Generate K mazes
    for (int k = 0; k < K; k++) {
        // Initialize maze cells with all walls up
        MazeCell** maze = new MazeCell*[M];
        for (int i = 0; i < M; i++) {
            maze[i] = new MazeCell[N];
            for (int j = 0; j < N; j++) {
                maze[i][j].x = i;
                maze[i][j].y = j;
                maze[i][j].leftWall = true;
                maze[i][j].rightWall = true;
                maze[i][j].upWall = true;
                maze[i][j].downWall = true;
                maze[i][j].visited = false;
            }
        }
        
        // Initialize stack and push starting cell (0,0)
        Stack<MazeCell*> stack;
        MazeCell* startingCell = &maze[0][0];
        startingCell->visited = true;
        stack.push(startingCell);
        
        // Generate the maze
        while (!stack.empty()) {
            // Get current cell and its coordinates
            MazeCell* currentCell = stack.pop();
            int i = currentCell->x;
            int j = currentCell->y;
            
            // Get a list of unvisited neighboring cells
            vector<MazeCell*> unvisitedNeighbors;
            if (i > 0 && !maze[i-1][j].visited) {
                unvisitedNeighbors.push_back(&maze[i-1][j]);
            }
            if (i < M-1 && !maze[i+1][j].visited) {
                unvisitedNeighbors.push_back(&maze[i+1][j]);
            }
            if (j > 0 && !maze[i][j-1].visited) {
                unvisitedNeighbors.push_back(&maze[i][j-1]);
            }
            if (j < N-1 && !maze[i][j+1].visited) {
                unvisitedNeighbors.push_back(&maze[i][j+1]);
            }
            
            if (!unvisitedNeighbors.empty()) {
                // Choose a random unvisited neighboring cell
                int randomIndex = rand() % unvisitedNeighbors.size();
                MazeCell* nextCell = unvisitedNeighbors[randomIndex];
                
                // Knock down the wall between the current and next cell
                if (nextCell == &maze[i-1][j]) {
                    currentCell->upWall = false;
                    nextCell->downWall = false;
                } else if (nextCell == &maze[i+1][j]) {
                    currentCell->downWall = false;
                    nextCell->upWall = false;
                } else if (nextCell == &maze[i][j-1]) {
                    currentCell->leftWall = false;
                    nextCell->rightWall = false;
                } else if (nextCell == &maze[i][j+1]) {
                    currentCell->rightWall = false;
                    nextCell->leftWall = false;
                }
                
                // Mark the next cell as visited and push it onto the stack
                nextCell->visited = true;
                stack.push(currentCell);
                stack.push(nextCell);
            }
        }
        string filename = "maze_" + to_string(k+1) + ".txt";
        
        // Open file
        ofstream fout(filename);
        if (!fout.is_open()) {
            cerr << "Failed to open file " << filename << endl;
            return;
        }
        fout << M << " " << N << endl;
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                fout << "x=" << j << " y=" << i;
                fout << " l=" << maze[i][j].leftWall;
                fout << " r=" << maze[i][j].rightWall;
                fout << " u=" << maze[i][j].upWall;
                fout << " d=" << maze[i][j].downWall << endl;
            }
        }
        
        fout.close();
    }
    cout << "All mazes are generated."<<endl;
    cout<<endl;
}

void pathFinder(int mazeID, int entryX, int entryY, int exitX, int exitY) {
    // Open the input maze file
    string mazeFileName = "maze_" + to_string(mazeID) + ".txt";
    ifstream fin(mazeFileName);
    if (!fin) {
        cerr << "Error: Cannot open file " << mazeFileName << endl;
        return;
    }
    
    // Read the maze dimensions
    int M, N;
    fin >> M >> N;
    
    // Create the maze
    MazeCell **maze = new MazeCell *[M];
    for (int i = 0; i < M; i++) {
        maze[i] = new MazeCell[N];
    }
    
    // Read the maze cells
    for (int i = 0; i < M * N; i++) {
        string line;
        getline(fin, line);

        // Parse the line to extract x, y, l, r, u, d
        int x, y, l, r, u, d;
        sscanf(line.c_str(), "x=%d y=%d l=%d r=%d u=%d d=%d", &x, &y, &l, &r, &u, &d);

        // Check that x and y are within bounds
        if ((x >= 0 && x < N) && (y >= 0 && y < M)) {
            maze[y][x].x = x;
            maze[y][x].y = y;
            maze[y][x].leftWall = (l == 1);
            maze[y][x].rightWall = (r == 1);
            maze[y][x].upWall = (u == 1);
            maze[y][x].downWall = (d == 1);
            maze[y][x].visited = false;
            
            // Set the next pointer to null for now
            maze[y][x].next = nullptr;
        }
        else {
            cerr << "Error: Invalid maze cell coordinates (" << x << "," << y << ")" << endl;
        }
    }
    
    // Set the next pointers for each cell
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            // Set the next pointer to the cell to the right, if it exists and there is no right wall
            if (j < N - 1 && !maze[i][j].rightWall) {
                maze[i][j].next = &maze[i][j+1];
            }
            // Set the next pointer to the cell below, if it exists and there is no down wall
            if (i < M - 1 && !maze[i][j].downWall) {
                maze[i][j].next = &maze[i+1][j];
            }
        }
    }
    
    // Close the input maze file
    fin.close();
    
    // Create the output path file
    string pathFileName = "maze_" + to_string(mazeID) + "_path_" + to_string(entryX) + "_" + to_string(entryY) + "_" + to_string(exitX) + "_" + to_string(exitY) + ".txt";
    ofstream fout(pathFileName);

    // Check that the output path file was opened successfully
    if (!fout) {
        cerr << "Error: Cannot create file " << pathFileName << endl;
        return;
    }
    
    // Find the entry and exit cells
    MazeCell *entryCell = &maze[entryY][entryX];
    MazeCell *exitCell = &maze[exitY][exitX];
    
    // Start with an initially empty stack
    Stack<MazeCell*> cellStack;
    
    // Push the entry cell to the stack
    entryCell->visited = true;
    cellStack.push(entryCell);
    
    // Use the top element in the stack to choose the next cell to visit
    while (!cellStack.empty()) {
        MazeCell *currentCell = cellStack.peek();
        
        // If the current cell is the exit cell, break out of the loop
        if (currentCell == exitCell) {
            break;
        }
        
        // Check the neighbors of the current cell
        bool foundUnVisitedNeighbor = false;
        for (int i = 0; i < 4; i++) {
            MazeCell *neighborCell = NULL;
            switch (i) {
                case 0: // Left
                    if (!currentCell->leftWall && currentCell->y > 0) {
                        neighborCell = &maze[currentCell->y - 1][currentCell->x];
                    }
                    break;
                case 1: // Right
                    if (!currentCell->rightWall && currentCell->y < M - 1) {
                        neighborCell = &maze[currentCell->y + 1][currentCell->x];
                    }
                    break;
                case 2: // Up
                    if (!currentCell->upWall && currentCell->x > 0) {
                        neighborCell = &maze[currentCell->y][currentCell->x - 1];
                    }
                    break;
                case 3: // Down
                    if (!currentCell->downWall && currentCell->x < N - 1) {
                        neighborCell = &maze[currentCell->y][currentCell->x + 1];
                    }
                    break;
            }
            if (neighborCell && !neighborCell->visited) {
                // Mark the neighbor as visited and add it to the stack
                neighborCell->visited = true;
                neighborCell->next = currentCell;
                cellStack.push(neighborCell);
                foundUnVisitedNeighbor = true;
                break;
            }
        }
        
        if (!foundUnVisitedNeighbor) {
            // If there are no unvisited neighbors, backtrack by popping the current cell from the stack
            cellStack.pop();
        }
    }
    
    // Trace the path from the exit cell to the entry cell
        MazeCell *currentCell = exitCell;
        while (currentCell != entryCell) {
            // Output the current cell to the path file if it is not null
            if (currentCell != nullptr) {
                fout << currentCell->x << " " << currentCell->y << endl;
            }
            
            // Move to the next cell if it is not null
            if (currentCell->next != nullptr) {
                currentCell = currentCell->next;
            } else {
                // Handle the case where currentCell->next is null
                break;
            }
        }

        
        // Output the entry cell to the path file if it is not null
        if (entryCell != nullptr) {
            fout << entryCell->x << " " << entryCell->y << endl;
        }
        
        // Close the output path file
        fout.close();

    
    // Free the memory allocated for the maze
    for (int i = 0; i < M; i++) {
        delete[] maze[i];
    }
    delete[] maze;
    return;
}
