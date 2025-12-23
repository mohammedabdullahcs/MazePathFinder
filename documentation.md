# User Manual & Program Interface

## Interface Overview

The Maze PathFinder application provides an interactive environment for visualizing maze generation and pathfinding algorithms. The interface is divided into three main sections:

1.  **Control Panel (Left):** Contains all user controls for generating mazes and running algorithms.
2.  **Visualization Area (Center):** The main grid where the maze generation and solving animations take place.
3.  **Metrics Panel (Right):** Displays real-time performance data and algorithm statistics.

![Application Interface](project.png)
*(Description: The interface features a control panel on the left with buttons and settings, a large central grid displaying the maze, and a metrics panel on the right showing step counts and execution time.)*

## Button Functions & Controls

*   **Generate Maze (Green Button):** Initiates the maze generation process using Kruskal's Algorithm. This clears any existing maze and builds a new perfect maze from scratch.
*   **Solve with BFS (Blue Button):** Starts the Breadth-First Search algorithm to find the shortest path from the start to the end node.
*   **Solve with DFS (Orange Button):** Starts the Depth-First Search algorithm to find a path (not necessarily the shortest) from the start to the end node.
*   **Clear Maze (Red Button):** Resets the grid, removing the maze structure and any pathfinding visualizations, returning to a blank state.
*   **Maze Size (Spinboxes):** Allows the user to define the dimensions of the maze.
    *   **Width:** Sets the number of columns in the grid.
    *   **Height:** Sets the number of rows in the grid.

## Understanding the Visualization

The grid uses a color-coded system to represent different states of the maze cells:

*   **Blue Square (Top-Left):** Represents the **Start** position.
*   **Red Square (Bottom-Right):** Represents the **End** (Goal) position.
*   **Yellow Squares:** Indicate cells that have been **explored** by the algorithm during the search process. This visualizes the "search space."
*   **Green Squares:** Highlight the **final path** found from the start to the end.

## Performance Metrics Panel (Right Side)

This panel provides quantitative data to analyze algorithm performance:

*   **Steps Taken:** The total number of cells visited/explored by the algorithm to find the target. This is a direct measure of the work done.
*   **Solve Time:** The time elapsed in milliseconds for the algorithm to complete.
*   **Time Complexity:** Displayed as $O(V + E)$, where $V$ is vertices (cells) and $E$ is edges (connections). This represents the theoretical upper bound of operations.
*   **Space Complexity:** Displayed as $O(V)$, representing the memory required to store the visited set and the queue/stack.

### Interpreting BFS vs DFS
*   **BFS:** Typically shows a "flooding" pattern (concentric expansion) in yellow. It guarantees the shortest path (green).
*   **DFS:** Typically shows a "wandering" pattern (long, winding paths) in yellow. It does not guarantee the shortest path and may take a much longer route.

---

# Disjoint Set (Union-Find) Data Structure

## What is Union-Find?

The Union-Find (or Disjoint Set Union - DSU) data structure tracks a set of elements partitioned into a number of disjoint (non-overlapping) subsets. It provides near-constant-time operations to:
1.  **Find** which subset a particular element belongs to.
2.  **Union** (merge) two subsets into a single subset.

**Real-world Analogy:** Think of a room full of people. Initially, everyone is a stranger (in their own group of 1). If person A shakes hands with person B, they form a group. If person B then shakes hands with person C, all three (A, B, C) become part of the same group. Union-Find efficiently tracks these connections to answer "Are A and C in the same group?"

## Core Operations

### 1. find(x)
Determines the representative (root) of the set containing element `x`.

*   **Path Compression Optimization:** During the `find` operation, we make the found root the direct parent of `x` and all ancestors of `x`. This flattens the tree structure, ensuring future operations are faster.
    *   *Without optimization:* Tree height can grow to $O(n)$, making operations slow.
    *   *With optimization:* Tree height remains very small.

**Pseudocode:**
```text
function find(x):
    if parent[x] != x:
        parent[x] = find(parent[x])  // Path compression
    return parent[x]
```

### 2. union(x, y)
Merges the set containing element `x` with the set containing element `y`.

*   **Union by Rank Optimization:** We always attach the shorter tree to the root of the taller tree. This prevents the tree from becoming skewed (like a linked list) and keeps the height logarithmic.

**Pseudocode:**
```text
function union(x, y):
    rootX = find(x)
    rootY = find(y)
    if rootX == rootY: return  // Already in same set
    
    if rank[rootX] < rank[rootY]:
        parent[rootX] = rootY
    else if rank[rootX] > rank[rootY]:
        parent[rootY] = rootX
    else:
        parent[rootY] = rootX
        rank[rootX] = rank[rootX] + 1
```

## Time Complexity Analysis

With both Path Compression and Union by Rank, the amortized time complexity for each operation is $O(\alpha(n))$, where $\alpha$ is the inverse Ackermann function.
*   For all practical values of $n$ (even up to the number of atoms in the universe), $\alpha(n) \le 4$.
*   **Practical Implication:** Operations are effectively **constant time**, $O(1)$.

## unionfind.h Code Structure

The header file defines the class interface, storing the `parent` array (to track connections) and `rank` array (to track tree depth).

```cpp
#ifndef UNIONFIND_H 
#define UNIONFIND_H 

#include <vector>

class UnionFind {
private:
    std::vector<int> parent;
    std::vector<int> rank;

public:
    UnionFind(int n);
    
    // Find the root of element x with path compression
    int find(int x);
    
    // Union two sets
    bool unite(int x, int y);
    
    // Check if two elements are in same set
    bool connected(int x, int y);
};

#endif // UNIONFIND_H
```

## unionfind.cpp Implementation

The implementation file contains the logic for initialization and the optimized core operations.

```cpp
#include "unionfind.h"

UnionFind::UnionFind(int n) {
    parent.resize(n);
    rank.resize(n, 0);
    
    // Each element is its own parent initially
    for (int i = 0; i < n; i++) {
        parent[i] = i;
    }
}

int UnionFind::find(int x) {
    // Path compression: make x point directly to root
    if (parent[x] != x) {
        parent[x] = find(parent[x]);
    }
    return parent[x];
}

bool UnionFind::unite(int x, int y) {
    int rootX = find(x);
    int rootY = find(y);
    
    // Already in same set
    if (rootX == rootY) {
        return false;
    }
    
    // Union by rank: attach smaller tree under larger
    if (rank[rootX] < rank[rootY]) {
        parent[rootX] = rootY;
    } else if (rank[rootX] > rank[rootY]) {
        parent[rootY] = rootX;
    } else {
        parent[rootY] = rootX;
        rank[rootX]++;
    }
    
    return true;
}

bool UnionFind::connected(int x, int y) {
    return find(x) == find(y);
}
```

---

# Kruskal's Algorithm & Maze Generation

## Kruskal's Algorithm Theory

Kruskal's Algorithm is a famous greedy algorithm originally designed to find the **Minimum Spanning Tree (MST)** of a weighted graph.

**Adaptation for Mazes:**
To generate a maze, we treat the grid of cells as a graph where edges are the walls between cells. A "perfect maze" is mathematically equivalent to a Spanning Tree of this grid graph.
*   **Vertices:** The cells of the maze.
*   **Edges:** The connections (passages) between cells.

**Algorithm Pseudocode:**
1.  Start with a grid where every cell is isolated by walls (all edges exist but are "closed").
2.  Create a list of all possible walls (edges) between adjacent cells.
3.  **Randomize** the order of walls (this replaces edge weights in the standard MST algorithm).
4.  Iterate through the shuffled wall list:
    *   Select a wall separating Cell A and Cell B.
    *   Use **Union-Find** to check if Cell A and Cell B are already connected.
    *   If they are **not** connected (different sets):
        *   Remove the wall (open the passage).
        *   **Union** the sets of Cell A and Cell B.
    *   If they are already connected, keep the wall (removing it would create a cycle).

## Why Kruskal for Mazes?

1.  **No Loops:** By definition, a tree has no cycles. This ensures the maze has no circular paths (unless we intentionally add them later).
2.  **Connectivity:** A spanning tree connects all vertices. This guarantees that every cell in the maze is reachable from the start.
3.  **High Entropy:** The randomization step ensures that the resulting maze structure is unpredictable and complex.

## Perfect Maze Definition

A **Perfect Maze** is defined by two properties:
1.  **Connectivity:** Every point in the maze is reachable from every other point.
2.  **Uniqueness:** There is exactly one unique path between any two points.

This structure forces pathfinding algorithms to explore significantly, as there are no "shortcuts" or loops to bypass dead ends.

## maze.h & Maze Generation

The `Maze` class manages the grid state and the generation logic. It uses the `Cell` struct to track walls for each unit and the `Wall` struct to represent connections.

```cpp
#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <queue>
#include "unionfind.h"

struct Cell {
    bool top = true;
    bool bottom = true;
    bool left = true;
    bool right = true;
};

struct Wall {
    int x1, y1, x2, y2;
    Wall(int a, int b, int c, int d) : x1(a), y1(b), x2(c), y2(d) {}
};

class Maze {
private:
    int width, height;
    std::vector<std::vector<Cell>> grid;
    std::vector<Wall> wallRemovalOrder;
    
public:
    Maze(int w, int h);
    
    // Generate maze using Randomized Kruskal's algorithm
    void generateMaze(int extraCycles = 0);
    
    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    Cell getCell(int x, int y) const;
    const std::vector<Wall>& getWallRemovalOrder() const { return wallRemovalOrder; }
    
    // Check if cell has wall in direction
    bool hasWall(int x, int y, int direction) const;
    // 0=top, 1=right, 2=bottom, 3=left
};

#endif // MAZE_H
```

## maze.cpp Implementation

The implementation initializes the grid, creates the wall list, shuffles it, and then applies the Union-Find logic to carve the maze.

**Time Complexity:** $O(W \log W)$ or $O(W \cdot \alpha(V))$, where $W$ is the number of walls. The dominant factor is usually the shuffling or the iteration over walls.

```cpp
#include "maze.h"
#include <algorithm>
#include <random>

Maze::Maze(int w, int h) : width(w), height(h) {
    grid.resize(h, std::vector<Cell>(w));
}

void Maze::generateMaze(int extraCycles) {
    wallRemovalOrder.clear();
    
    // Initialize all cells with all walls
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = Cell();
        }
    }
    
    // Create list of all possible walls
    std::vector<Wall> walls;
    
    // Horizontal walls (between rows)
    for (int y = 0; y < height - 1; y++) {
        for (int x = 0; x < width; x++) {
            walls.push_back(Wall(x, y, x, y + 1));
        }
    }
    
    // Vertical walls (between columns)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width - 1; x++) {
            walls.push_back(Wall(x, y, x + 1, y));
        }
    }
    
    // Shuffle walls randomly using modern C++ random
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(walls.begin(), walls.end(), gen);
    
    // List to keep track of walls we didn't remove
    std::vector<Wall> skippedWalls;

    // Union-Find for Kruskal's algorithm
    UnionFind uf(width * height);
    
    // Process each wall
    for (const Wall& wall : walls) {
        int cell1 = wall.y1 * width + wall.x1;
        int cell2 = wall.y2 * width + wall.x2;
        
        // If cells are not connected, remove wall and unite them
        if (!uf.connected(cell1, cell2)) {
            uf.unite(cell1, cell2);
            wallRemovalOrder.push_back(wall);
            
            // Remove wall from grid
            if (wall.x1 == wall.x2) {
                // Vertical wall between (x1,y1) and (x1,y2)
                if (wall.y1 < wall.y2) {
                    grid[wall.y1][wall.x1].bottom = false;
                    grid[wall.y2][wall.x2].top = false;
                } else {
                    grid[wall.y2][wall.x2].bottom = false;
                    grid[wall.y1][wall.x1].top = false;
                }
            } else {
                // Horizontal wall between (x1,y1) and (x2,y1)
                if (wall.x1 < wall.x2) {
                    grid[wall.y1][wall.x1].right = false;
                    grid[wall.y2][wall.x2].left = false;
                } else {
                    grid[wall.y2][wall.x2].right = false;
                    grid[wall.y1][wall.x1].left = false;
                }
            }
            // removeWallFromGrid(wall); // Refactored helper or inline code
        } else {
            // save(Keep track of skipped) walls for later
            skippedWalls.push_back(wall);
        }
    }

    // NEW: Add cycles by removing some skipped walls
    std::shuffle(skippedWalls.begin(), skippedWalls.end(), gen);

    for (int i = 0; i < extraCycles && i < skippedWalls.size(); i++) {
        Wall wall = skippedWalls[i];
        wallRemovalOrder.push_back(wall); // Add to animation order
        
        // Remove wall from grid (copy logic from above)
        if (wall.x1 == wall.x2) {
             // Vertical wall logic...
             if (wall.y1 < wall.y2) {
                grid[wall.y1][wall.x1].bottom = false;
                grid[wall.y2][wall.x2].top = false;
            } else {
                grid[wall.y2][wall.x2].bottom = false;
                grid[wall.y1][wall.x1].top = false;
            }
        } else {
            // Horizontal wall logic...
            if (wall.x1 < wall.x2) {
                grid[wall.y1][wall.x1].right = false;
                grid[wall.y2][wall.x2].left = false;
            } else {
                grid[wall.y2][wall.x2].right = false;
                grid[wall.y1][wall.x1].left = false;
            }
        }
    }
}

Cell Maze::getCell(int x, int y) const {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return grid[y][x];
    }
    return Cell();
}

bool Maze::hasWall(int x, int y, int direction) const {
    if (x < 0 || x >= width || y < 0 || y >= height) {
        return true;
    }
    
    switch (direction) {
        case 0: return grid[y][x].top;      // top
        case 1: return grid[y][x].right;    // right
        case 2: return grid[y][x].bottom;   // bottom
        case 3: return grid[y][x].left;     // left
    }
    return true;
}
```

---

# Pathfinding: BFS vs DFS

## Breadth-First Search (BFS) Theory

BFS is a graph traversal algorithm that explores neighbors level by level. It uses a **Queue** (FIFO - First In, First Out) data structure.

*   **Mechanism:** It starts at the root and explores all neighbor nodes at the present depth prior to moving on to the nodes at the next depth level.
*   **Visualization:** It expands like a ripple in a pond or concentric circles radiating from the start.
*   **Guarantee:** In an unweighted graph (like our maze), BFS **always** finds the shortest path.

**Complexity:**
*   Time: $O(V + E)$
*   Space: $O(V)$ (to store the queue)

## Depth-First Search (DFS) Theory

DFS is a graph traversal algorithm that explores as far as possible along each branch before backtracking. It uses a **Stack** (LIFO - Last In, First Out) data structure.

*   **Mechanism:** It chooses a path and follows it until it hits a dead end, then backtracks to the most recent junction to try a different path.
*   **Visualization:** It looks like a snake winding deeply into the maze.
*   **Guarantee:** DFS does **not** guarantee the shortest path. It often finds a "good enough" path but can take a very long, winding route.

**Complexity:**
*   Time: $O(V + E)$
*   Space: $O(V)$ (to store the recursion stack or explicit stack)

## BFS vs DFS Comparison Table

| Property | Breadth-First Search (BFS) | Depth-First Search (DFS) |
| :--- | :--- | :--- |
| **Data Structure** | Queue (FIFO) | Stack (LIFO) |
| **Shortest Path** | **Guaranteed** | Not Guaranteed |
| **Exploration Pattern** | Wide / Concentric | Deep / Linear |
| **Memory Usage** | Higher (stores all nodes at current level) | Lower (stores only current path nodes) |
| **Best Use Case** | Finding shortest path | Solving puzzles, topological sort |

## pathfinder.h Structure

The `PathFinder` class encapsulates the solving logic. It returns a `PathResult` struct containing the path, the order of exploration (for animation), and statistics.

```cpp
#ifndef PATHFINDER_H
#define PATHFINDER_H

#include <vector>
#include <queue>
#include <stack>
#include "maze.h"

struct PathResult {
    std::vector<std::pair<int, int>> path;
    std::vector<std::pair<int, int>> explored;  // Cells visited during search
    int stepsCount = 0;
    bool found = false;
};

class PathFinder {
private:
    const Maze* maze;
    int startX, startY, endX, endY;
    
    bool isValid(int x, int y) const;
    std::vector<std::pair<int, int>> getNeighbors(int x, int y) const;
    
public:
    PathFinder(const Maze* m, int sx, int sy, int ex, int ey);
    
    // BFS - finds shortest path
    PathResult solveBFS();
    
    // DFS - explores depth-first
    PathResult solveDFS();
};

#endif // PATHFINDER_H
```

## pathfinder.cpp Implementation

Both algorithms share a similar structure but differ in the data structure used (Queue vs Stack).

**Key Implementation Details:**
*   `visited[][]`: A boolean matrix to prevent processing the same cell twice (avoids infinite loops).
*   `parent[][]`: Stores the coordinate of the cell that led to the current cell. This is crucial for **path reconstruction**—once the goal is reached, we backtrack from Goal $\to$ Parent $\to$ Parent... $\to$ Start.

```cpp
#include "pathfinder.h"
#include <algorithm> // it contains std::reverse that's used in path reconstruction
#include <cstdlib> // for std::rand, std::srand that might be used in pathfinding variations
#include <ctime> // for std::time to seed random number generator

PathFinder::PathFinder(const Maze* m, int sx, int sy, int ex, int ey)
    : maze(m), startX(sx), startY(sy), endX(ex), endY(ey) {}

bool PathFinder::isValid(int x, int y) const {
    return x >= 0 && x < maze->getWidth() && y >= 0 && y < maze->getHeight();
}

std::vector<std::pair<int, int>> PathFinder::getNeighbors(int x, int y) const {
    std::vector<std::pair<int, int>> neighbors;
    
    // Check all 4 directions: top, right, bottom, left
    if (!maze->hasWall(x, y, 0) && isValid(x, y - 1)) 
        neighbors.push_back({x, y - 1});
    if (!maze->hasWall(x, y, 1) && isValid(x + 1, y)) 
        neighbors.push_back({x + 1, y});
    if (!maze->hasWall(x, y, 2) && isValid(x, y + 1)) 
        neighbors.push_back({x, y + 1});
    if (!maze->hasWall(x, y, 3) && isValid(x - 1, y)) 
        neighbors.push_back({x - 1, y});
    
    return neighbors;
}

PathResult PathFinder::solveBFS() {
    PathResult result;
    
    std::vector<std::vector<bool>> visited(maze->getHeight(), 
                                           std::vector<bool>(maze->getWidth(), false));
    std::vector<std::vector<std::pair<int, int>>> parent(maze->getHeight(),
                                                         std::vector<std::pair<int, int>>(maze->getWidth(), {-1, -1}));
    
    std::queue<std::pair<int, int>> q;
    q.push({startX, startY});
    visited[startY][startX] = true;
    result.explored.push_back({startX, startY});
    
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        result.stepsCount++;
        
        if (x == endX && y == endY) {
            result.found = true;
            
            // Reconstruct path
            int cx = endX, cy = endY;
            while (cx != -1 && cy != -1) {
                result.path.push_back({cx, cy});
                auto [px, py] = parent[cy][cx];
                cx = px;
                cy = py;
            }
            std::reverse(result.path.begin(), result.path.end());
            return result;
        }
        
        for (auto [nx, ny] : getNeighbors(x, y)) {
            if (!visited[ny][nx]) {
                visited[ny][nx] = true;
                parent[ny][nx] = {x, y};
                q.push({nx, ny});
                result.explored.push_back({nx, ny});
            }
        }
    }
    
    return result;
}

PathResult PathFinder::solveDFS() {
    PathResult result;
    
    std::vector<std::vector<bool>> visited(maze->getHeight(), 
                                           std::vector<bool>(maze->getWidth(), false));
    std::vector<std::vector<std::pair<int, int>>> parent(maze->getHeight(),
                                                         std::vector<std::pair<int, int>>(maze->getWidth(), {-1, -1}));
    
    std::stack<std::pair<int, int>> st;
    st.push({startX, startY});
    visited[startY][startX] = true;
    result.explored.push_back({startX, startY});
    
    while (!st.empty()) {
        auto [x, y] = st.top();
        st.pop();
        result.stepsCount++;
        
        if (x == endX && y == endY) {
            result.found = true;
            
            // Reconstruct path
            int cx = endX, cy = endY;
            while (cx != -1 && cy != -1) {
                result.path.push_back({cx, cy});
                auto [px, py] = parent[cy][cx];
                cx = px;
                cy = py;
            }
            std::reverse(result.path.begin(), result.path.end());
            return result;
        }
        
        // Get neighbors and reverse them for DFS (different exploration order than BFS)
        auto neighbors = getNeighbors(x, y);
        std::reverse(neighbors.begin(), neighbors.end());
        
        for (auto [nx, ny] : neighbors) {
            if (!visited[ny][nx]) {
                visited[ny][nx] = true;
                parent[ny][nx] = {x, y};
                st.push({nx, ny});
                result.explored.push_back({nx, ny});
            }
        }
    }
    
    return result;
}
```

---

# Analysis & Observations

## Why Maze Structure Affects Algorithm Performance

The structure of a "Perfect Maze" (generated by Kruskal's) has significant implications for pathfinding:
1.  **Tree Structure:** Since the maze is a tree, there are no cycles. This means there is only **one** unique path between the start and end.
2.  **Dead Ends:** Perfect mazes are full of dead ends.
3.  **Impact on BFS:** BFS will explore all branches equally until it hits the correct path. In a dense maze, this often means visiting a large percentage of the total cells ($O(V)$).
4.  **Impact on DFS:** DFS might get "lucky" and pick the correct branch early, solving it very quickly. Or, it might get "unlucky" and explore every single dead end before finding the correct path.

## Program Design Decisions

*   **Separation of Concerns:** The algorithmic logic (`UnionFind`, `Maze`, `PathFinder`) is completely decoupled from the GUI code (`MainWindow`, `MazeScene`). This adheres to the Model-View-Controller (MVC) pattern, making the code testable and reusable.
*   **Animation:** We chose to animate the generation and solving steps rather than just showing the result. This provides educational value, allowing users to *see* the algorithm's "thought process."
*   **Metrics:** Measuring steps and time provides empirical evidence to back up the theoretical Big-O complexity analysis.

## Expected Results

| Metric | BFS | DFS |
| :--- | :--- | :--- |
| **Steps Taken** | High (Explores widely) | Variable (High variance) |
| **Execution Time** | Generally Faster (finds goal efficiently) | Variable (can be very slow if unlucky) |
| **Path Length** | **Optimal (Shortest)** | Sub-optimal (often much longer) |

> **Key Takeaway:** While DFS is memory-efficient, BFS is generally preferred for pathfinding in unweighted grids because it guarantees the shortest path. However, the visual "wandering" of DFS provides a fascinating insight into recursive backtracking strategies.
