#include "tsp_solver.h"
#include <cstring>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <climits>

TSPSolver::TSPSolver(Graph* g) : graph(g)
{
    if (g) {
        numCities = g->getNumCities();
        // Copy distance matrix
        for (int i = 0; i < numCities; i++) {
            for (int j = 0; j < numCities; j++) {
                distance[i][j] = g->getDistance(i, j);
            }
        }
    } else {
        numCities = 0;
    }
}

TSPSolver::~TSPSolver()
{
}

void TSPSolver::setGraph(Graph* g)
{
    graph = g;
    if (g) {
        numCities = g->getNumCities();
        for (int i = 0; i < numCities; i++) {
            for (int j = 0; j < numCities; j++) {
                distance[i][j] = g->getDistance(i, j);
            }
        }
    }
}

Graph* TSPSolver::getGraph() const
{
    return graph;
}

void TSPSolver::copyPath(int* source, int* dest, int length)
{
    for (int i = 0; i < length; i++) {
        dest[i] = source[i];
    }
}

int TSPSolver::calculatePathDistance(int* path, int pathLength)
{
    int totalDistance = 0;
    for (int i = 0; i < pathLength - 1; i++) {
        totalDistance += distance[path[i]][path[i + 1]];
    }
    // Add distance from last city back to first city
    totalDistance += distance[path[pathLength - 1]][path[0]];
    return totalDistance;
}

bool TSPSolver::isValidPath(int* path, int pathLength)
{
    if (pathLength != numCities) {
        return false;
    }

    bool visited[20] = {false};
    for (int i = 0; i < pathLength; i++) {
        if (path[i] < 0 || path[i] >= numCities || visited[path[i]]) {
            return false;
        }
        visited[path[i]] = true;
    }
    return true;
}

void TSPSolver::permute(int* path, int l, int r, int& bestCost, int* bestPath)
{
    if (l == r) {
        int cost = calculatePathDistance(path, numCities);
        if (cost < bestCost) {
            bestCost = cost;
            copyPath(path, bestPath, numCities);
        }
    } else {
        for (int i = l; i <= r; i++) {
            // Swap
            int temp = path[l];
            path[l] = path[i];
            path[i] = temp;

            permute(path, l + 1, r, bestCost, bestPath);

            // Swap back
            temp = path[l];
            path[l] = path[i];
            path[i] = temp;
        }
    }
}

TSPResult TSPSolver::solveBruteForce()
{
    TSPResult result;
    result.pathLength = numCities;

    auto start = std::chrono::high_resolution_clock::now();

    // Initialize path
    int path[20];
    for (int i = 0; i < numCities; i++) {
        path[i] = i;
    }

    int bestCost = INT_MAX;
    int bestPath[20];

    // Generate all permutations starting from city 0
    permute(path, 1, numCities - 1, bestCost, bestPath);

    copyPath(bestPath, result.path, numCities);
    result.totalDistance = bestCost;

    auto end = std::chrono::high_resolution_clock::now();
    result.executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    result.memoryUsed = sizeof(result);

    return result;
}

TSPResult TSPSolver::solveGreedyNearestNeighbor()
{
    TSPResult result;
    result.pathLength = numCities;

    auto start = std::chrono::high_resolution_clock::now();

    int path[20];
    bool visited[20] = {false};

    // Start from city 0
    path[0] = 0;
    visited[0] = true;
    int currentCity = 0;

    // Greedily select nearest unvisited city
    for (int i = 1; i < numCities; i++) {
        int nearestCity = -1;
        int minDistance = INT_MAX;

        // FIX: Check that distance > 0 (valid edge exists)
        for (int j = 0; j < numCities; j++) {
            if (!visited[j] && distance[currentCity][j] > 0 &&
                distance[currentCity][j] < minDistance) {
                minDistance = distance[currentCity][j];
                nearestCity = j;
            }
        }

        if (nearestCity == -1) {
            // If no unvisited city with valid edge found, 
            // try to pick any unvisited city with valid edge
            for (int j = 0; j < numCities; j++) {
                if (!visited[j] && distance[currentCity][j] > 0) {
                    nearestCity = j;
                    break;
                }
            }
        }

        // SAFETY: If still no city found (incomplete graph), 
        // this indicates disconnected graph
        if (nearestCity == -1) {
            // Pick any unvisited city (graph is disconnected)
            for (int j = 0; j < numCities; j++) {
                if (!visited[j]) {
                    nearestCity = j;
                    break;
                }
            }
        }

        path[i] = nearestCity;
        visited[nearestCity] = true;
        currentCity = nearestCity;
    }

    copyPath(path, result.path, numCities);
    result.totalDistance = calculatePathDistance(result.path, numCities);

    auto end = std::chrono::high_resolution_clock::now();
    result.executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    result.memoryUsed = sizeof(result);

    return result;
}

TSPResult TSPSolver::solveNearestInsertion()
{
    TSPResult result;
    result.pathLength = numCities;

    auto start = std::chrono::high_resolution_clock::now();

    int path[20];
    bool inPath[20] = {false};
    int pathSize = 0;

    // Start with city 0
    path[0] = 0;
    inPath[0] = true;
    pathSize = 1;

    if (numCities > 1) {
        // Find nearest city to city 0 with valid edge (distance > 0)
        int nearestToZero = -1;
        int minDist = INT_MAX;
        for (int j = 1; j < numCities; j++) {
            if (distance[0][j] > 0 && distance[0][j] < minDist) {
                minDist = distance[0][j];
                nearestToZero = j;
            }
        }
        if (nearestToZero == -1) {
            nearestToZero = 1;  // Fallback
        }

        path[1] = nearestToZero;
        inPath[nearestToZero] = true;
        pathSize = 2;
    }

    // Insert remaining cities
    while (pathSize < numCities) {
        int bestCity = -1;
        int bestPosition = -1;
        int minIncrease = INT_MAX;

        // Find nearest city not in path
        for (int i = 0; i < numCities; i++) {
            if (!inPath[i]) {
                // Find best position to insert this city
                for (int j = 0; j < pathSize; j++) {
                    int nextJ = (j + 1) % pathSize;
                    int increase = distance[path[j]][i] + distance[i][path[nextJ]] -
                                   distance[path[j]][path[nextJ]];

                    if (increase < minIncrease) {
                        minIncrease = increase;
                        bestCity = i;
                        bestPosition = j + 1;
                    }
                }
            }
        }

        if (bestCity == -1) {
            // If no city found, just add the first unvisited
            for (int i = 0; i < numCities; i++) {
                if (!inPath[i]) {
                    bestCity = i;
                    bestPosition = pathSize;
                    break;
                }
            }
        }

        // Insert bestCity at bestPosition
        for (int i = pathSize; i > bestPosition; i--) {
            path[i] = path[i - 1];
        }
        path[bestPosition] = bestCity;
        inPath[bestCity] = true;
        pathSize++;
    }

    copyPath(path, result.path, numCities);
    result.totalDistance = calculatePathDistance(result.path, numCities);

    auto end = std::chrono::high_resolution_clock::now();
    result.executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    result.memoryUsed = sizeof(result);

    return result;
}
