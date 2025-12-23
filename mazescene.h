#ifndef MAZESCENE_H
#define MAZESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include "maze.h"
#include "pathfinder.h"

class MazeScene : public QGraphicsScene {
    Q_OBJECT

private:
    Maze* maze;
    int cellSize;
    int animationStep;
    int maxSteps;
    QTimer* animationTimer;
    
    // Pathfinding visualization
    PathResult currentPath;
    int pathStep;
    bool showingPath;
    QString solvingAlgorithm;  // "BFS" or "DFS"
    
public:
    MazeScene(int w, int h, QObject* parent = nullptr);
    ~MazeScene();
    
    void generateNewMaze();
    void solveMazeWithBFS();
    void solveMazeWithDFS();
    void clearSolution();
    void resetMaze();
    
    Maze* getMaze() { return maze; }
    PathResult getCurrentPath() const { return currentPath; }

private slots:
    void animateGeneration();
    void animatePathfinding();
    
private:
    void drawMaze();
    void drawCell(int x, int y, const QColor& color);
};

#endif // MAZESCENE_H