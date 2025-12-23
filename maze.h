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
    void reset();
    
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