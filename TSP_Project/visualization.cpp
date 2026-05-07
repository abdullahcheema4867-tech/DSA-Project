#include "visualization.h"
#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <cmath>

Visualization::Visualization(QWidget *parent)
    : QWidget(parent), graph(nullptr), pathLength(0), showPath(false)
{
    setMinimumSize(600, 500);
    setStyleSheet("background-color: white;");
}

void Visualization::setGraph(Graph* g)
{
    graph = g;
    if (g && g->getNumCities() > 0) {
        // Auto-arrange cities in a circle if coordinates are not set
        int radius = 150;
        int centerX = 300;
        int centerY = 250;
        int numCities = g->getNumCities();

        for (int i = 0; i < numCities; i++) {
            if (g->getCityX(i) == 0 && g->getCityY(i) == 0) {
                double angle = 2 * 3.14159265359 * i / numCities;
                int x = centerX + static_cast<int>(radius * cos(angle));
                int y = centerY + static_cast<int>(radius * sin(angle));
                // Note: In real implementation, you'd need to update coordinates
            }
        }
    }
    update();
}

void Visualization::setPath(int* path, int length)
{
    if (path && length > 0 && length <= 20) {
        for (int i = 0; i < length; i++) {
            currentPath[i] = path[i];
        }
        pathLength = length;
        showPath = true;
    }
    update();
}

void Visualization::setShowPath(bool show)
{
    showPath = show;
    update();
}

void Visualization::clearVisualization()
{
    graph = nullptr;
    pathLength = 0;
    showPath = false;
    update();
}

void Visualization::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    if (!graph || graph->getNumCities() == 0) {
        painter.drawText(50, 50, "No graph to display. Please create a graph first.");
        return;
    }

    drawEdges(painter);
    if (showPath) {
        drawPath(painter);
    }
    drawCities(painter);
}

void Visualization::drawCities(QPainter &painter)
{
    if (!graph) return;

    int numCities = graph->getNumCities();
    painter.setFont(QFont("Arial", 10, QFont::Bold));

    for (int i = 0; i < numCities; i++) {
        int x = graph->getCityX(i);
        int y = graph->getCityY(i);

        // If coordinates are 0, arrange in circle
        if (x == 0 && y == 0) {
            double angle = 2 * 3.14159265359 * i / numCities;
            int radius = 150;
            x = 300 + static_cast<int>(radius * cos(angle));
            y = 250 + static_cast<int>(radius * sin(angle));
        }

        // Draw circle
        painter.setBrush(QBrush(Qt::lightBlue));
        painter.setPen(QPen(Qt::blue, 2));
        painter.drawEllipse(x - CITY_RADIUS, y - CITY_RADIUS, 2 * CITY_RADIUS, 2 * CITY_RADIUS);

        // Draw city name
        char cityName = graph->getCityName(i);
        painter.setPen(Qt::black);
        painter.drawText(x - 5, y + 5, QString(cityName));
    }
}

void Visualization::drawEdges(QPainter &painter)
{
    if (!graph) return;

    int numCities = graph->getNumCities();
    painter.setPen(QPen(Qt::gray, 1));

    for (int i = 0; i < numCities; i++) {
        for (int j = i + 1; j < numCities; j++) {
            int dist = graph->getDistance(i, j);
            if (dist > 0) {
                int x1 = graph->getCityX(i);
                int y1 = graph->getCityY(i);
                int x2 = graph->getCityX(j);
                int y2 = graph->getCityY(j);

                // If coordinates are 0, arrange in circle
                if (x1 == 0 && y1 == 0) {
                    double angle = 2 * 3.14159265359 * i / numCities;
                    int radius = 150;
                    x1 = 300 + static_cast<int>(radius * cos(angle));
                    y1 = 250 + static_cast<int>(radius * sin(angle));
                }
                if (x2 == 0 && y2 == 0) {
                    double angle = 2 * 3.14159265359 * j / numCities;
                    int radius = 150;
                    x2 = 300 + static_cast<int>(radius * cos(angle));
                    y2 = 250 + static_cast<int>(radius * sin(angle));
                }

                painter.drawLine(x1, y1, x2, y2);

                // Draw distance label
                int midX = (x1 + x2) / 2;
                int midY = (y1 + y2) / 2;
                painter.drawText(midX + 5, midY + 5, QString::number(dist));
            }
        }
    }
}

void Visualization::drawPath(QPainter &painter)
{
    if (!graph || pathLength == 0) return;

    painter.setPen(QPen(Qt::red, 3));

    for (int i = 0; i < pathLength; i++) {
        int currentCity = currentPath[i];
        int nextCity = currentPath[(i + 1) % pathLength];

        int x1 = graph->getCityX(currentCity);
        int y1 = graph->getCityY(currentCity);
        int x2 = graph->getCityX(nextCity);
        int y2 = graph->getCityY(nextCity);

        // If coordinates are 0, arrange in circle
        if (x1 == 0 && y1 == 0) {
            double angle = 2 * 3.14159265359 * currentCity / graph->getNumCities();
            int radius = 150;
            x1 = 300 + static_cast<int>(radius * cos(angle));
            y1 = 250 + static_cast<int>(radius * sin(angle));
        }
        if (x2 == 0 && y2 == 0) {
            double angle = 2 * 3.14159265359 * nextCity / graph->getNumCities();
            int radius = 150;
            x2 = 300 + static_cast<int>(radius * cos(angle));
            y2 = 250 + static_cast<int>(radius * sin(angle));
        }

        painter.drawLine(x1, y1, x2, y2);
    }
}
