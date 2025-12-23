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