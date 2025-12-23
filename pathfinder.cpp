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