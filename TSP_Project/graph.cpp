#include "graph.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>

Graph::Graph() : numCities(0)
{
    // Initialize adjacency matrix with 0s
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            adjacencyMatrix[i][j] = 0;
        }
    }
}

Graph::~Graph()
{
    clear();
}

bool Graph::addCity(char cityName, int x, int y)
{
    if (numCities >= MAX_CITIES) {
        return false;
    }

    // Check if city already exists
    for (int i = 0; i < numCities; i++) {
        if (cities[i].cityName == cityName) {
            return false;
        }
    }

    cities[numCities].cityName = cityName;
    cities[numCities].x = x;
    cities[numCities].y = y;
    numCities++;
    return true;
}

bool Graph::addEdge(int cityIndex1, int cityIndex2, int distance)
{
    if (cityIndex1 < 0 || cityIndex1 >= numCities ||
        cityIndex2 < 0 || cityIndex2 >= numCities) {
        return false;
    }

    adjacencyMatrix[cityIndex1][cityIndex2] = distance;
    adjacencyMatrix[cityIndex2][cityIndex1] = distance;  // For undirected graph
    return true;
}

bool Graph::addEdgeByName(char city1, char city2, int distance)
{
    int index1 = getCityIndex(city1);
    int index2 = getCityIndex(city2);

    if (index1 == -1 || index2 == -1) {
        return false;
    }

    return addEdge(index1, index2, distance);
}

int Graph::getNumCities() const
{
    return numCities;
}

int Graph::getDistance(int city1, int city2) const
{
    if (city1 < 0 || city1 >= numCities || city2 < 0 || city2 >= numCities) {
        return 0;
    }
    return adjacencyMatrix[city1][city2];
}

char Graph::getCityName(int index) const
{
    if (index < 0 || index >= numCities) {
        return '?';
    }
    return cities[index].cityName;
}

int Graph::getCityX(int index) const
{
    if (index < 0 || index >= numCities) {
        return 0;
    }
    return cities[index].x;
}

int Graph::getCityY(int index) const
{
    if (index < 0 || index >= numCities) {
        return 0;
    }
    return cities[index].y;
}

int Graph::getCityIndex(char cityName) const
{
    for (int i = 0; i < numCities; i++) {
        if (cities[i].cityName == cityName) {
            return i;
        }
    }
    return -1;
}

void Graph::displayGraph() const
{
    printf("\n=== Graph Information ===\n");
    printf("Number of Cities: %d\n", numCities);
    printf("Cities: ");
    for (int i = 0; i < numCities; i++) {
        printf("%c ", cities[i].cityName);
    }
    printf("\n");
}

void Graph::displayAdjacencyMatrix() const
{
    printf("\n=== Adjacency Matrix ===\n");
    printf("     ");
    for (int i = 0; i < numCities; i++) {
        printf("%c   ", cities[i].cityName);
    }
    printf("\n");

    for (int i = 0; i < numCities; i++) {
        printf("%c   ", cities[i].cityName);
        for (int j = 0; j < numCities; j++) {
            printf("%3d ", adjacencyMatrix[i][j]);
        }
        printf("\n");
    }
}

bool Graph::loadFromFile(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file) {
        return false;
    }

    int n;
    fscanf(file, "%d", &n);

    // Add cities
    for (int i = 0; i < n; i++) {
        char cityName;
        int x, y;
        fscanf(file, " %c %d %d", &cityName, &x, &y);
        addCity(cityName, x, y);
    }

    // Add distances
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int distance;
            fscanf(file, "%d", &distance);
            if (i < j) {  // Only read upper triangle for undirected graph
                adjacencyMatrix[i][j] = distance;
                adjacencyMatrix[j][i] = distance;
            }
        }
    }

    fclose(file);
    return true;
}

bool Graph::saveToFile(const char* filename) const
{
    FILE* file = fopen(filename, "w");
    if (!file) {
        return false;
    }

    fprintf(file, "%d\n", numCities);

    // Save cities
    for (int i = 0; i < numCities; i++) {
        fprintf(file, "%c %d %d\n", cities[i].cityName, cities[i].x, cities[i].y);
    }

    // Save adjacency matrix
    for (int i = 0; i < numCities; i++) {
        for (int j = 0; j < numCities; j++) {
            fprintf(file, "%d ", adjacencyMatrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
    return true;
}

void Graph::clear()
{
    numCities = 0;
    for (int i = 0; i < MAX_CITIES; i++) {
        for (int j = 0; j < MAX_CITIES; j++) {
            adjacencyMatrix[i][j] = 0;
        }
    }
}

const int** Graph::getAdjacencyMatrix() const
{
    // This is a workaround; in real scenario, you might want to use a different approach
    static const int* rows[20];
    for (int i = 0; i < numCities; i++) {
        rows[i] = adjacencyMatrix[i];
    }
    return rows;
}
