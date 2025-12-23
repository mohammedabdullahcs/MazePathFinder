#include "maze.h"
#include <algorithm>
#include <random>

Maze::Maze(int w, int h) : width(w), height(h) {
    grid.resize(h, std::vector<Cell>(w));
}

void Maze::reset() {
    wallRemovalOrder.clear();
    
    // Initialize all cells with all walls
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            grid[y][x] = Cell();
        }
    }
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