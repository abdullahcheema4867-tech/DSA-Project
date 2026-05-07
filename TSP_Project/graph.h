#ifndef GRAPH_H
#define GRAPH_H

struct CityNode
{
    char cityName;
    int x;  // x coordinate for visualization
    int y;  // y coordinate for visualization
};

class Graph
{
private:
    static const int MAX_CITIES = 20;
    int adjacencyMatrix[MAX_CITIES][MAX_CITIES];  // Static representation
    CityNode cities[MAX_CITIES];
    int numCities;

public:
    Graph();
    ~Graph();

    // Graph operations
    bool addCity(char cityName, int x = 0, int y = 0);
    bool addEdge(int cityIndex1, int cityIndex2, int distance);
    bool addEdgeByName(char city1, char city2, int distance);

    // Getters
    int getNumCities() const;
    int getDistance(int city1, int city2) const;
    char getCityName(int index) const;
    int getCityX(int index) const;
    int getCityY(int index) const;
    int getCityIndex(char cityName) const;

    // Display
    void displayGraph() const;
    void displayAdjacencyMatrix() const;

    // File operations
    bool loadFromFile(const char* filename);
    bool saveToFile(const char* filename) const;

    // Clear
    void clear();

    // Get matrix for algorithms
    const int** getAdjacencyMatrix() const;
};

#endif // GRAPH_H
