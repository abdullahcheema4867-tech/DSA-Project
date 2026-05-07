#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QTextEdit>
#include <cstdio>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    graph = new Graph();
    solver = new TSPSolver(graph);

    // Create visualization widget
    visualization = new Visualization();
    ui->layoutVisualization->addWidget(visualization);
    visualization->setGraph(graph);

    // Connect signals
    connect(ui->btnAddCity, &QPushButton::clicked, this, &MainWindow::on_btnAddCity_clicked);
    connect(ui->btnAddEdge, &QPushButton::clicked, this, &MainWindow::on_btnAddEdge_clicked);
    connect(ui->btnSolve, &QPushButton::clicked, this, &MainWindow::on_btnSolve_clicked);
    connect(ui->btnLoadFile, &QPushButton::clicked, this, &MainWindow::on_btnLoadFile_clicked);
    connect(ui->btnSaveFile, &QPushButton::clicked, this, &MainWindow::on_btnSaveFile_clicked);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::on_btnClear_clicked);
    connect(ui->btnCreateSample, &QPushButton::clicked, this, &MainWindow::on_btnCreateSample_clicked);
    connect(ui->cmbAlgorithm, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::on_cmbAlgorithm_currentIndexChanged);

    setWindowTitle("TSP Solver - Qt Application");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete graph;
    delete solver;
    delete visualization;
}

void MainWindow::on_btnAddCity_clicked()
{
    QString cityName = ui->lineEditCityName->text().trimmed();

    if (cityName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a city name.");
        return;
    }

    if (cityName.length() > 1) {
        cityName = cityName.left(1);
    }

    if (graph->addCity(cityName.at(0).toLatin1())) {
        ui->textEditLog->append(QString("City added: %1").arg(cityName));
        ui->lineEditCityName->clear();
        updateCityList();
        visualization->setGraph(graph);
    } else {
        QMessageBox::warning(this, "Error", "Could not add city. City may already exist or limit reached.");
    }
}

void MainWindow::on_btnAddEdge_clicked()
{
    QString city1Str = ui->lineEditCity1->text().trimmed();
    QString city2Str = ui->lineEditCity2->text().trimmed();
    QString distanceStr = ui->lineEditDistance->text().trimmed();

    if (city1Str.isEmpty() || city2Str.isEmpty() || distanceStr.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill all fields.");
        return;
    }

    char city1 = city1Str.at(0).toLatin1();
    char city2 = city2Str.at(0).toLatin1();
    int distance = distanceStr.toInt();

    if (distance <= 0) {
        QMessageBox::warning(this, "Input Error", "Distance must be positive.");
        return;
    }

    if (graph->addEdgeByName(city1, city2, distance)) {
        ui->textEditLog->append(QString("Edge added: %1 <-> %2 (Distance: %3)").arg(city1).arg(city2).arg(distance));
        ui->lineEditCity1->clear();
        ui->lineEditCity2->clear();
        ui->lineEditDistance->clear();
        visualization->setGraph(graph);
    } else {
        QMessageBox::warning(this, "Error", "Could not add edge. Cities may not exist.");
    }
}

void MainWindow::on_btnSolve_clicked()
{
    if (graph->getNumCities() == 0) {
        QMessageBox::warning(this, "Error", "Please create a graph first.");
        return;
    }

    solver->setGraph(graph);

    int selectedAlgorithm = ui->cmbAlgorithm->currentIndex();

    ui->textEditLog->append("\n=== Solving TSP ===");

    switch (selectedAlgorithm) {
    case 0:  // Brute Force
        ui->textEditLog->append("Algorithm: Brute Force (Exhaustive Search)");
        lastResult = solver->solveBruteForce();
        break;
    case 1:  // Greedy
        ui->textEditLog->append("Algorithm: Greedy (Nearest Neighbor)");
        lastResult = solver->solveGreedyNearestNeighbor();
        break;
    case 2:  // Nearest Insertion
        ui->textEditLog->append("Algorithm: Nearest Insertion");
        lastResult = solver->solveNearestInsertion();
        break;
    default:
        lastResult = solver->solveBruteForce();
    }

    displayResult(lastResult);
    visualization->setPath(lastResult.path, lastResult.pathLength);
}

void MainWindow::displayResult(const TSPResult& result)
{
    ui->textEditLog->append("\n--- Results ---");

    // Display path
    QString pathStr = "Optimal Path: ";
    for (int i = 0; i < result.pathLength; i++) {
        pathStr += QString("%1 → ").arg(graph->getCityName(result.path[i]));
    }
    pathStr += QString("%1").arg(graph->getCityName(result.path[0]));
    ui->textEditLog->append(pathStr);

    // Display metrics
    ui->textEditLog->append(QString("Total Distance: %1").arg(result.totalDistance));
    ui->textEditLog->append(QString("Execution Time: %1 microseconds").arg(result.executionTime));
    ui->textEditLog->append(QString("Memory Used: %1 bytes").arg(result.memoryUsed));
}

void MainWindow::on_btnLoadFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open Graph File", "", "Text Files (*.txt)");

    if (!fileName.isEmpty()) {
        if (graph->loadFromFile(fileName.toStdString().c_str())) {
            ui->textEditLog->append(QString("Graph loaded from: %1").arg(fileName));
            updateCityList();
            visualization->setGraph(graph);
        } else {
            QMessageBox::warning(this, "Error", "Could not load file.");
        }
    }
}

void MainWindow::on_btnSaveFile_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save Graph File", "", "Text Files (*.txt)");

    if (!fileName.isEmpty()) {
        if (graph->saveToFile(fileName.toStdString().c_str())) {
            ui->textEditLog->append(QString("Graph saved to: %1").arg(fileName));
        } else {
            QMessageBox::warning(this, "Error", "Could not save file.");
        }
    }
}

void MainWindow::on_btnClear_clicked()
{
    graph->clear();
    ui->textEditLog->clear();
    ui->textEditLog->append("Graph cleared.");
    updateCityList();
    visualization->clearVisualization();
}

void MainWindow::on_btnCreateSample_clicked()
{
    graph->clear();

    // Create sample graph with 4 cities
    graph->addCity('A', 100, 150);
    graph->addCity('B', 400, 100);
    graph->addCity('C', 400, 300);
    graph->addCity('D', 100, 300);

    // Add edges (distances)
    graph->addEdge(0, 1, 10);  // A-B
    graph->addEdge(0, 2, 15);  // A-C
    graph->addEdge(0, 3, 20);  // A-D
    graph->addEdge(1, 2, 35);  // B-C
    graph->addEdge(1, 3, 25);  // B-D
    graph->addEdge(2, 3, 30);  // C-D

    ui->textEditLog->append("\n=== Sample Graph Created ===");
    ui->textEditLog->append("Cities: A, B, C, D");
    ui->textEditLog->append("A ↔ B: 10");
    ui->textEditLog->append("A ↔ C: 15");
    ui->textEditLog->append("A ↔ D: 20");
    ui->textEditLog->append("B ↔ C: 35");
    ui->textEditLog->append("B ↔ D: 25");
    ui->textEditLog->append("C ↔ D: 30");

    updateCityList();
    visualization->setGraph(graph);
}

void MainWindow::on_cmbAlgorithm_currentIndexChanged(int index)
{
    QString algoName;
    QString description;

    switch (index) {
    case 0:
        algoName = "Brute Force";
        description = "Exhaustive search - checks all permutations. Best for small graphs. O(n!)";
        break;
    case 1:
        algoName = "Greedy (Nearest Neighbor)";
        description = "Selects nearest unvisited city. Fast but may not be optimal. O(n²)";
        break;
    case 2:
        algoName = "Nearest Insertion";
        description = "Inserts cities at best positions. Better than greedy. O(n²)";
        break;
    default:
        algoName = "Unknown";
    }

    ui->textEditLog->append(QString("\nSelected Algorithm: %1").arg(algoName));
    ui->textEditLog->append(QString("Description: %1").arg(description));
}

void MainWindow::updateCityList()
{
    int numCities = graph->getNumCities();
    QString cities = "Cities: ";

    for (int i = 0; i < numCities; i++) {
        cities += QString("%1 ").arg(graph->getCityName(i));
    }

    ui->labelCityCount->setText(QString("Total Cities: %1").arg(numCities));
    ui->textEditLog->append(cities);
}
