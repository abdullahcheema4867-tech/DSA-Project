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
    
    // CRITICAL: Verify ALL edges in path exist (distance > 0)
    for (int i = 0; i < pathLength; i++) {
        int fromCity = path[i];
        int toCity = path[(i + 1) % pathLength];
        if (distance[fromCity][toCity] == 0) {
            return false;  // Edge doesn't exist!
        }
    }
    
    return true;
}

void TSPSolver::permute(int* path, int l, int r, int& bestCost, int* bestPath)
{
    if (l == r) {
        // CRITICAL: Check if path is valid (all edges exist)
        if (isValidPath(path, numCities)) {
            int cost = calculatePathDistance(path, numCities);
            if (cost < bestCost) {
                bestCost = cost;
                copyPath(path, bestPath, numCities);
            }
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

    if (bestCost == INT_MAX) {
        // No valid path found (graph is disconnected)
        result.totalDistance = -1;  // Error indicator
        for (int i = 0; i < numCities; i++) {
            result.path[i] = i;
        }
    } else {
        copyPath(bestPath, result.path, numCities);
        result.totalDistance = bestCost;
    }

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

        // CRITICAL: Only consider cities with VALID edges (distance > 0)
        for (int j = 0; j < numCities; j++) {
            if (!visited[j] && distance[currentCity][j] > 0 &&
                distance[currentCity][j] < minDistance) {
                minDistance = distance[currentCity][j];
                nearestCity = j;
            }
        }

        // If no valid edge found, graph is disconnected - ERROR
        if (nearestCity == -1) {
            result.totalDistance = -1;  // Error indicator
            for (int k = 0; k < numCities; k++) {
                result.path[k] = k;
            }
            auto end = std::chrono::high_resolution_clock::now();
            result.executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            result.memoryUsed = sizeof(result);
            return result;  // Exit early
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
        // Find nearest city to city 0 WITH VALID EDGE (distance > 0)
        int nearestToZero = -1;
        int minDist = INT_MAX;
        for (int j = 1; j < numCities; j++) {
            if (distance[0][j] > 0 && distance[0][j] < minDist) {
                minDist = distance[0][j];
                nearestToZero = j;
            }
        }
        
        if (nearestToZero == -1) {
            // No valid edge from city 0 - graph is disconnected
            result.totalDistance = -1;
            for (int k = 0; k < numCities; k++) {
                result.path[k] = k;
            }
            auto end = std::chrono::high_resolution_clock::now();
            result.executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            result.memoryUsed = sizeof(result);
            return result;
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

        // Find city not in path with best insertion position
        for (int i = 0; i < numCities; i++) {
            if (!inPath[i]) {
                // Find best position to insert this city
                for (int j = 0; j < pathSize; j++) {
                    int nextJ = (j + 1) % pathSize;
                    int cityA = path[j];
                    int cityB = path[nextJ];
                    
                    // CRITICAL: Check if edges exist
                    if (distance[cityA][i] == 0 || distance[i][cityB] == 0) {
                        continue;  // Skip invalid edges
                    }
                    
                    int increase = distance[cityA][i] + distance[i][cityB] -
                                   distance[cityA][cityB];

                    if (increase < minIncrease) {
                        minIncrease = increase;
                        bestCity = i;
                        bestPosition = j + 1;
                    }
                }
            }
        }

        if (bestCity == -1) {
            // No valid insertion found - graph is disconnected
            result.totalDistance = -1;
            for (int k = 0; k < numCities; k++) {
                result.path[k] = k;
            }
            auto end = std::chrono::high_resolution_clock::now();
            result.executionTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
            result.memoryUsed = sizeof(result);
            return result;
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
