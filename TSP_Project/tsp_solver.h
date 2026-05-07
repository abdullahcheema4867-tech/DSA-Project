#ifndef TSP_SOLVER_H
#define TSP_SOLVER_H

#include "graph.h"

struct TSPResult
{
    int path[20];           // Path of cities
    int pathLength;         // Number of cities in path
    int totalDistance;      // Total distance of path
    long long executionTime; // Execution time in microseconds
    long long memoryUsed;   // Memory used in bytes
};

class TSPSolver
{
private:
    Graph* graph;
    int numCities;
    int distance[20][20];

    // Helper functions
    void copyPath(int* source, int* dest, int length);
    int calculatePathDistance(int* path, int pathLength);
    bool isValidPath(int* path, int pathLength);
    void permute(int* path, int l, int r, int& bestCost, int* bestPath);

public:
    TSPSolver(Graph* g);
    ~TSPSolver();

    // Algorithm implementations
    TSPResult solveBruteForce();
    TSPResult solveGreedyNearestNeighbor();
    TSPResult solveNearestInsertion();

    // Utility methods
    void setGraph(Graph* g);
    Graph* getGraph() const;
};

#endif // TSP_SOLVER_H
