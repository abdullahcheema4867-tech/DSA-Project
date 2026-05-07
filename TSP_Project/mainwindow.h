#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "graph.h"
#include "tsp_solver.h"
#include "visualization.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddCity_clicked();
    void on_btnAddEdge_clicked();
    void on_btnSolve_clicked();
    void on_btnLoadFile_clicked();
    void on_btnSaveFile_clicked();
    void on_btnClear_clicked();
    void on_btnCreateSample_clicked();
    void on_cmbAlgorithm_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;
    Graph* graph;
    TSPSolver* solver;
    Visualization* visualization;
    TSPResult lastResult;

    void updateCityList();
    void displayResult(const TSPResult& result);
    void createSampleGraph();
};

#endif // MAINWINDOW_H
