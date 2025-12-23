#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QSpinBox>
#include "mazescene.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

private:
    MazeScene* mazeScene;
    QGraphicsView* graphicsView;
    
    // UI Elements
    QSpinBox* widthSpinBox;
    QSpinBox* heightSpinBox;
    QPushButton* generateBtn;
    QPushButton* bfsBtn;
    QPushButton* dfsBtn;
    QPushButton* clearBtn;
    QPushButton* deleteBtn;
    QLabel* performanceLabel;
    QLabel* titleLabel;
    
    int mazeWidth, mazeHeight;
    int bfsSteps, dfsSteps;
    int bfsTime, dfsTime;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onGenerateClicked();
    void onBFSClicked();
    void onDFSClicked();
    void onClearClicked();
    void onDeleteClicked();
    void updateStats();
    
private:
    void setupUI();
    void connectSignals();
};

#endif // MAINWINDOW_H