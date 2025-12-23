#include "mazescene.h"
#include <QPainter>
#include <QGraphicsLineItem>

MazeScene::MazeScene(int w, int h, QObject* parent)
    : QGraphicsScene(parent), cellSize(40), animationStep(0), 
      pathStep(0), showingPath(false) {
    
    maze = new Maze(w, h);
    animationTimer = new QTimer(this);
    connect(animationTimer, &QTimer::timeout, this, &MazeScene::animateGeneration);
    
    setSceneRect(0, 0, w * cellSize, h * cellSize);
    drawMaze();
}

MazeScene::~MazeScene() {
    delete maze;
}

void MazeScene::generateNewMaze() {
    clear();
    maze->generateMaze(maze->getWidth() * maze->getHeight() / 20); // Add some cycles
    
    const auto& walls = maze->getWallRemovalOrder();
    maxSteps = walls.size();
    animationStep = 0;
    showingPath = false;
    
    // Draw initial maze with all walls
    for (int y = 0; y < maze->getHeight(); y++) {
        for (int x = 0; x < maze->getWidth(); x++) {
            drawCell(x, y, Qt::white);
        }
    }
    
    // Start animation
    animationTimer->start(10);
}

void MazeScene::animateGeneration() {
    if (animationStep >= maxSteps) {
        animationTimer->stop();
        return;
    }
    
    const auto& walls = maze->getWallRemovalOrder();
    
    // Highlight the wall being removed
    const Wall& wall = walls[animationStep];
    drawCell(wall.x1, wall.y1, QColor(200, 200, 255));
    drawCell(wall.x2, wall.y2, QColor(200, 200, 255));
    
    animationStep++;
}

void MazeScene::solveMazeWithBFS() {
    if (animationTimer->isActive()) return;
    
    // Clear previous solution visualization
    clear();
    drawMaze();
    
    showingPath = true;
    solvingAlgorithm = "BFS";
    pathStep = 0;
    
    int startX = 0, startY = 0;
    int endX = maze->getWidth() - 1, endY = maze->getHeight() - 1;
    
    // Draw start and end points before animation
    drawCell(startX, startY, QColor(0, 0, 255));
    drawCell(endX, endY, QColor(255, 0, 0));
    
    PathFinder pf(maze, startX, startY, endX, endY);
    currentPath = pf.solveBFS();
    
    animationTimer->disconnect();
    connect(animationTimer, &QTimer::timeout, this, &MazeScene::animatePathfinding);
    animationTimer->start(50);
}

void MazeScene::solveMazeWithDFS() {
    if (animationTimer->isActive()) return;
    
    // Clear previous solution visualization
    clear();
    drawMaze();
    
    showingPath = true;
    solvingAlgorithm = "DFS";
    pathStep = 0;
    
    int startX = 0, startY = 0;
    int endX = maze->getWidth() - 1, endY = maze->getHeight() - 1;
    
    // Draw start and end points before animation
    drawCell(startX, startY, QColor(0, 0, 255));
    drawCell(endX, endY, QColor(255, 0, 0));
    
    PathFinder pf(maze, startX, startY, endX, endY);
    currentPath = pf.solveDFS();
    
    animationTimer->disconnect();
    connect(animationTimer, &QTimer::timeout, this, &MazeScene::animatePathfinding);
    animationTimer->start(50);
}

void MazeScene::animatePathfinding() {
    if (pathStep >= static_cast<int>(currentPath.explored.size())) {
        // Draw final path
        for (auto [x, y] : currentPath.path) {
            drawCell(x, y, QColor(0, 255, 0));
        }
        
        // Draw start and end
        drawCell(0, 0, QColor(0, 0, 255));
        drawCell(maze->getWidth() - 1, maze->getHeight() - 1, QColor(255, 0, 0));
        
        animationTimer->stop();
        return;
    }
    
    auto [x, y] = currentPath.explored[pathStep];
    drawCell(x, y, QColor(255, 200, 0));
    pathStep++;
}

void MazeScene::drawMaze() {
    // Draw all cells with walls
    for (int y = 0; y < maze->getHeight(); y++) {
        for (int x = 0; x < maze->getWidth(); x++) {
            drawCell(x, y, Qt::white);
        }
    }
}

void MazeScene::resetMaze() {
    if (animationTimer->isActive()) {
        animationTimer->stop();
    }
    
    showingPath = false;
    currentPath = PathResult();
    pathStep = 0;
    
    maze->reset();
    
    clear();
    // Draw initial maze with all walls
    for (int y = 0; y < maze->getHeight(); y++) {
        for (int x = 0; x < maze->getWidth(); x++) {
            drawCell(x, y, Qt::white);
        }
    }
}

void MazeScene::clearSolution() {
    if (animationTimer->isActive()) {
        animationTimer->stop();
    }
    
    showingPath = false;
    currentPath = PathResult();
    pathStep = 0;
    
    // Redraw maze with white cells only (no colored overlays)
    for (int y = 0; y < maze->getHeight(); y++) {
        for (int x = 0; x < maze->getWidth(); x++) {
            drawCell(x, y, Qt::white);
        }
    }
}

void MazeScene::drawCell(int x, int y, const QColor& color) {
    int px = x * cellSize;
    int py = y * cellSize;
    
    // Draw cell rectangle
    auto rect = addRect(px, py, cellSize, cellSize, QPen(Qt::black), QBrush(color));
    
    // Draw walls
    Cell cell = maze->getCell(x, y);
    int lineWidth = 2;
    
    if (cell.top)
        addLine(px, py, px + cellSize, py, QPen(Qt::black, lineWidth));
    if (cell.bottom)
        addLine(px, py + cellSize, px + cellSize, py + cellSize, QPen(Qt::black, lineWidth));
    if (cell.left)
        addLine(px, py, px, py + cellSize, QPen(Qt::black, lineWidth));
    if (cell.right)
        addLine(px + cellSize, py, px + cellSize, py + cellSize, QPen(Qt::black, lineWidth));
}