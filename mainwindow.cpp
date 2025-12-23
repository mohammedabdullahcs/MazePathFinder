#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QGroupBox>
#include <QLabel>
#include <QFont>
#include <QMessageBox>
#include <QElapsedTimer>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), mazeWidth(15), mazeHeight(15), 
      bfsSteps(0), dfsSteps(0), bfsTime(0), dfsTime(0) {
    
    setupUI();
    connectSignals();
    
    setWindowTitle("Maze Generator & Pathfinding Suite");
    setGeometry(100, 100, 1600, 800);
    
    // Set dark background and dark text
    setStyleSheet(
        "QMainWindow { background-color: #f0f0f0; }"
        "QLabel { color: #1a1a1a; }"
        "QGroupBox { color: #1a1a1a; font-weight: bold; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
        "QPushButton { color: white; }"
        "QSpinBox { color: #1a1a1a; background-color: white; border: 1px solid #ccc; }"
    );
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI() {
    // Central widget
    QWidget* central = new QWidget(this);
    central->setStyleSheet("background-color: #f0f0f0;");
    setCentralWidget(central);
    
    QHBoxLayout* mainLayout = new QHBoxLayout(central);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // LEFT PANEL - Controls
    QWidget* controlPanel = new QWidget();
    controlPanel->setStyleSheet("background-color: #e8e8e8; padding: 15px; border-radius: 4px;");
    QVBoxLayout* controlLayout = new QVBoxLayout(controlPanel);
    
    // Title
    titleLabel = new QLabel("Maze Generator");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(14);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: #000000; margin-bottom: 10px;");
    controlLayout->addWidget(titleLabel);
    
    // Maze size controls
    QGroupBox* sizeGroup = new QGroupBox("Maze Size");
    sizeGroup->setStyleSheet(
        "QGroupBox { background-color: #ffffff; color: #000000; border: 1px solid #ddd; border-radius: 4px; padding: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* sizeLayout = new QVBoxLayout();
    
    QHBoxLayout* widthLayout = new QHBoxLayout();
    QLabel* widthLabel = new QLabel("Width:");
    widthLabel->setStyleSheet("color: #000000;");
    widthLayout->addWidget(widthLabel);
    widthSpinBox = new QSpinBox();
    widthSpinBox->setMinimum(5);
    widthSpinBox->setMaximum(50);
    widthSpinBox->setValue(mazeWidth);
    widthSpinBox->setStyleSheet("color: #000000; background-color: white; padding: 5px;");
    widthLayout->addWidget(widthSpinBox);
    sizeLayout->addLayout(widthLayout);
    
    QHBoxLayout* heightLayout = new QHBoxLayout();
    QLabel* heightLabel = new QLabel("Height:");
    heightLabel->setStyleSheet("color: #000000;");
    heightLayout->addWidget(heightLabel);
    heightSpinBox = new QSpinBox();
    heightSpinBox->setMinimum(5);
    heightSpinBox->setMaximum(50);
    heightSpinBox->setValue(mazeHeight);
    heightSpinBox->setStyleSheet("color: #000000; background-color: white; padding: 5px;");
    heightLayout->addWidget(heightSpinBox);
    sizeLayout->addLayout(heightLayout);
    
    sizeGroup->setLayout(sizeLayout);
    controlLayout->addWidget(sizeGroup);
    
    // Generate button
    generateBtn = new QPushButton("Generate Maze");
    generateBtn->setMinimumHeight(40);
    generateBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #4CAF50;"
        "  color: white;"
        "  font-weight: bold;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 10px;"
        "  font-size: 12px;"
        "}"
        "QPushButton:hover { background-color: #45a049; }"
        "QPushButton:pressed { background-color: #3d8b40; }"
    );
    controlLayout->addWidget(generateBtn);
    
    // Solve buttons
    QGroupBox* solveGroup = new QGroupBox("Solve Maze");
    solveGroup->setStyleSheet(
        "QGroupBox { background-color: #ffffff; color: #000000; border: 1px solid #ddd; border-radius: 4px; padding: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* solveLayout = new QVBoxLayout();
    
    bfsBtn = new QPushButton("Solve with BFS");
    bfsBtn->setMinimumHeight(38);
    bfsBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #2196F3;"
        "  color: white;"
        "  font-weight: bold;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 8px;"
        "  font-size: 11px;"
        "}"
        "QPushButton:hover { background-color: #0b7dda; }"
        "QPushButton:pressed { background-color: #0056b3; }"
    );
    solveLayout->addWidget(bfsBtn);
    
    dfsBtn = new QPushButton("Solve with DFS");
    dfsBtn->setMinimumHeight(38);
    dfsBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #FF9800;"
        "  color: white;"
        "  font-weight: bold;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 8px;"
        "  font-size: 11px;"
        "}"
        "QPushButton:hover { background-color: #e68900; }"
        "QPushButton:pressed { background-color: #cc7700; }"
    );
    solveLayout->addWidget(dfsBtn);
    
    // Clear buttons
    QHBoxLayout* clearLayout = new QHBoxLayout();
    
    clearBtn = new QPushButton("Clear Path");
    clearBtn->setMinimumHeight(38);
    clearBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #607D8B;"
        "  color: white;"
        "  font-weight: bold;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 8px;"
        "  font-size: 11px;"
        "}"
        "QPushButton:hover { background-color: #546E7A; }"
        "QPushButton:pressed { background-color: #455A64; }"
    );
    clearLayout->addWidget(clearBtn);
    
    deleteBtn = new QPushButton("Delete Maze");
    deleteBtn->setMinimumHeight(38);
    deleteBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #f44336;"
        "  color: white;"
        "  font-weight: bold;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 8px;"
        "  font-size: 11px;"
        "}"
        "QPushButton:hover { background-color: #da190b; }"
        "QPushButton:pressed { background-color: #ba0000; }"
    );
    clearLayout->addWidget(deleteBtn);
    
    solveLayout->addLayout(clearLayout);
    
    solveGroup->setLayout(solveLayout);
    controlLayout->addWidget(solveGroup);
    
    // Color Legend
    QGroupBox* legendGroup = new QGroupBox("Color Legend");
    legendGroup->setStyleSheet(
        "QGroupBox { background-color: #ffffff; color: #000000; border: 1px solid #ddd; border-radius: 4px; padding: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* legendLayout = new QVBoxLayout();
    
    QLabel* legendLabel = new QLabel(
        "🟦 Blue = Start\n"
        "🟥 Red = End\n"
        "🟨 Yellow = Explored\n"
        "🟩 Green = Final Path"
    );
    legendLabel->setFont(QFont("Courier", 9));
    legendLabel->setStyleSheet("color: #000000;");
    legendLayout->addWidget(legendLabel);
    legendGroup->setLayout(legendLayout);
    controlLayout->addWidget(legendGroup);
    
    controlLayout->addStretch();
    controlPanel->setMaximumWidth(280);
    controlPanel->setMinimumWidth(250);
    
    mainLayout->addWidget(controlPanel);
    
    // CENTER - Maze visualization
    graphicsView = new QGraphicsView();
    graphicsView->setStyleSheet("background-color: white; border: 1px solid #ddd; border-radius: 4px;");
    mazeScene = new MazeScene(mazeWidth, mazeHeight);
    graphicsView->setScene(mazeScene);
    
    mainLayout->addWidget(graphicsView, 1);
    
    // RIGHT PANEL - Performance Metrics
    QWidget* metricsPanel = new QWidget();
    metricsPanel->setStyleSheet("background-color: #e8e8e8; padding: 15px; border-radius: 4px;");
    QVBoxLayout* metricsLayout = new QVBoxLayout(metricsPanel);
    
    QLabel* metricsTitle = new QLabel("Performance Metrics");
    QFont metricsTitleFont = metricsTitle->font();
    metricsTitleFont.setPointSize(12);
    metricsTitleFont.setBold(true);
    metricsTitle->setFont(metricsTitleFont);
    metricsTitle->setStyleSheet("color: #000000; margin-bottom: 10px;");
    metricsLayout->addWidget(metricsTitle);
    
    QGroupBox* metricsGroup = new QGroupBox("Algorithm Stats");
    metricsGroup->setStyleSheet(
        "QGroupBox { background-color: #ffffff; color: #000000; border: 1px solid #ddd; border-radius: 4px; padding: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px; }"
    );
    QVBoxLayout* metricsGroupLayout = new QVBoxLayout();
    
    performanceLabel = new QLabel(
        "BFS Algorithm:\n"
        "  Steps Taken: --\n"
        "  Solve Time: -- ms\n"
        "  Time Complexity: O(V + E)\n"
        "  Space Complexity: O(V)\n"
        "\n"
        "DFS Algorithm:\n"
        "  Steps Taken: --\n"
        "  Solve Time: -- ms\n"
        "  Time Complexity: O(V + E)\n"
        "  Space Complexity: O(V)"
    );
    performanceLabel->setFont(QFont("Courier", 8));
    performanceLabel->setStyleSheet("color: #000000;");
    performanceLabel->setWordWrap(true);
    metricsGroupLayout->addWidget(performanceLabel);
    metricsGroup->setLayout(metricsGroupLayout);
    metricsLayout->addWidget(metricsGroup);
    
    metricsLayout->addStretch();
    metricsPanel->setMaximumWidth(280);
    metricsPanel->setMinimumWidth(250);
    
    mainLayout->addWidget(metricsPanel);
}

void MainWindow::connectSignals() {
    connect(generateBtn, &QPushButton::clicked, this, &MainWindow::onGenerateClicked);
    connect(bfsBtn, &QPushButton::clicked, this, &MainWindow::onBFSClicked);
    connect(dfsBtn, &QPushButton::clicked, this, &MainWindow::onDFSClicked);
    connect(clearBtn, &QPushButton::clicked, this, &MainWindow::onClearClicked);
    connect(deleteBtn, &QPushButton::clicked, this, &MainWindow::onDeleteClicked);
}

void MainWindow::onGenerateClicked() {
    mazeWidth = widthSpinBox->value();
    mazeHeight = heightSpinBox->value();
    
    delete mazeScene;
    mazeScene = new MazeScene(mazeWidth, mazeHeight);
    graphicsView->setScene(mazeScene);
    
    bfsSteps = 0;
    dfsSteps = 0;
    bfsTime = 0;
    dfsTime = 0;
    
    mazeScene->generateNewMaze();
    updateStats();
}

void MainWindow::onBFSClicked() {
    if (!mazeScene->getMaze()) {
        performanceLabel->setText("Generate a maze first!");
        return;
    }
    
    QElapsedTimer timer;
    timer.start();
    
    mazeScene->solveMazeWithBFS();
    
    bfsTime = timer.elapsed();
    bfsSteps = mazeScene->getCurrentPath().stepsCount;
    
    updateStats();
}

void MainWindow::onDFSClicked() {
    if (!mazeScene->getMaze()) {
        performanceLabel->setText("Generate a maze first!");
        return;
    }
    
    QElapsedTimer timer;
    timer.start();
    
    mazeScene->solveMazeWithDFS();
    
    dfsTime = timer.elapsed();
    dfsSteps = mazeScene->getCurrentPath().stepsCount;
    
    updateStats();
}

void MainWindow::onClearClicked() {
    if (!mazeScene->getMaze()) return;
    
    mazeScene->clearSolution();
    bfsSteps = 0;
    dfsSteps = 0;
    bfsTime = 0;
    dfsTime = 0;
    updateStats();
}

void MainWindow::onDeleteClicked() {
    if (!mazeScene->getMaze()) return;
    
    mazeScene->resetMaze();
    bfsSteps = 0;
    dfsSteps = 0;
    bfsTime = 0;
    dfsTime = 0;
    updateStats();
}

void MainWindow::updateStats() {
    PathResult path = mazeScene->getCurrentPath();
    
    QString stats = QString(
        "BFS Algorithm:\n"
        "  Steps Taken: %1\n"
        "  Solve Time: %2 ms\n"
        "  Time Complexity: O(V + E)\n"
        "  Space Complexity: O(V)\n"
        "\n"
        "DFS Algorithm:\n"
        "  Steps Taken: %3\n"
        "  Solve Time: %4 ms\n"
        "  Time Complexity: O(V + E)\n"
        "  Space Complexity: O(V)"
    ).arg(bfsSteps)
     .arg(bfsTime)
     .arg(dfsSteps)
     .arg(dfsTime);
    
    performanceLabel->setText(stats);
    performanceLabel->setStyleSheet("color: #000000;");
}