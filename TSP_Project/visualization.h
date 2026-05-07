#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include "graph.h"
#include "tsp_solver.h"

class Visualization : public QWidget
{
    Q_OBJECT

public:
    explicit Visualization(QWidget *parent = nullptr);

    void setGraph(Graph* g);
    void setPath(int* path, int pathLength);
    void setShowPath(bool show);
    void clearVisualization();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Graph* graph;
    int currentPath[20];
    int pathLength;
    bool showPath;
    static const int CITY_RADIUS = 20;
    static const int MARGIN = 50;

    void drawCities(QPainter &painter);
    void drawEdges(QPainter &painter);
    void drawPath(QPainter &painter);
};

#endif // VISUALIZATION_H
