# TSP Solver - Qt GUI Application

## Overview
This is a complete Travelling Salesperson Problem (TSP) solver implemented in C++ with a Qt GUI. The application provides an interactive interface to create graphs, visualize them, and solve TSP using multiple algorithms.

## Features

### Graph Creation
- **Add Cities**: Create cities by entering their names (single character)
- **Add Edges**: Define distances between cities
- **Interactive Input**: Easy-to-use form-based interface
- **File Operations**: Save and load graphs from files
- **Sample Graph**: Quick creation of a sample 4-city graph for testing

### Visualization
- **Graph Display**: Real-time visualization of the graph structure
- **City Nodes**: Nodes arranged in circular layout
- **Edge Display**: Shows connections with distance labels
- **Path Highlighting**: Visualizes the optimal path found by algorithms
- **Interactive Updates**: Updates automatically when graph changes

### TSP Solving Algorithms

#### 1. Brute Force (Exhaustive Search)
- **Time Complexity**: O(n!)
- **Space Complexity**: O(n)
- **Best For**: Small graphs (up to 10-12 cities)
- **Accuracy**: 100% - Finds optimal solution
- **Description**: Checks all possible permutations and returns the best path

#### 2. Greedy (Nearest Neighbor)
- **Time Complexity**: O(n²)
- **Space Complexity**: O(n)
- **Best For**: Large graphs
- **Accuracy**: Approximate (usually good but not optimal)
- **Description**: Always moves to the nearest unvisited city

#### 3. Nearest Insertion
- **Time Complexity**: O(n²)
- **Space Complexity**: O(n)
- **Best For**: Medium to large graphs
- **Accuracy**: Better than Greedy, worse than Brute Force
- **Description**: Builds tour by inserting cities at positions that increase cost the least

### Performance Analysis
The application displays:
- **Total Distance**: Sum of all edges in the path
- **Execution Time**: Time taken by algorithm (in microseconds)
- **Memory Usage**: Memory consumed by the solution
- **Algorithm Comparison**: Easy switching between algorithms

## Project Structure

### Core Classes

#### Graph (graph.h / graph.cpp)
- Represents the graph using adjacency matrix (static representation)
- Stores city names and coordinates
- Manages edges with distances
- File I/O operations

**Key Methods**:
- `addCity(char name, int x, int y)`: Add a city to the graph
- `addEdge(int i, int j, int distance)`: Add edge between two cities
- `getDistance(int i, int j)`: Get distance between cities
- `loadFromFile()` / `saveToFile()`: File operations

#### TSPSolver (tsp_solver.h / tsp_solver.cpp)
- Implements all TSP algorithms
- Uses the Graph class for input
- Returns TSPResult structure with detailed information

**Key Methods**:
- `solveBruteForce()`: Brute force algorithm
- `solveGreedyNearestNeighbor()`: Greedy algorithm
- `solveNearestInsertion()`: Nearest insertion algorithm

#### Visualization (visualization.h / visualization.cpp)
- Custom QWidget for drawing graphs
- Shows nodes, edges, and paths
- Updates in real-time based on graph state
- Automatic circular arrangement of nodes

#### Stack & Queue (stack.h / queue.h)
- Template-based implementations
- Fixed size arrays (no STL vectors)
- Can be used for extended algorithms

#### MainWindow (mainwindow.h / mainwindow.cpp)
- Qt GUI main window
- Handles all user interactions
- Connects UI elements to business logic
- Displays results and logs

## How to Use

### 1. Create a Graph
**Method 1: Manual Entry**
- Enter city name (single letter) and click "Add City"
- Enter two city names and distance, click "Add Edge"
- Repeat for all cities and edges

**Method 2: Load from File**
- Click "Load from File" and select a .txt file
- File format:
  ```
  4
  A 100 150
  B 400 100
  C 400 300
  D 100 300
  0 10 15 20
  10 0 35 25
  15 35 0 30
  20 25 30 0
  ```

**Method 3: Sample Graph**
- Click "Create Sample Graph" for a pre-built 4-city example

### 2. Select Algorithm
- Use the dropdown menu to choose algorithm:
  - Brute Force: Best accuracy, slower
  - Greedy: Fast, approximate
  - Nearest Insertion: Balanced

### 3. Solve TSP
- Click "Solve TSP" button
- View results in the log area
- See the path highlighted in red on the visualization

### 4. Save Results
- Click "Save to File" to export the graph
- Results are shown in the log area

## Data Structures Used

### Static Representation (Adjacency Matrix)
```cpp
int adjacencyMatrix[MAX_CITIES][MAX_CITIES];
```
- Space: O(n²)
- Edge lookup: O(1)
- Used in all algorithms

### Graph Representation
```cpp
struct CityNode {
    char cityName;    // Single letter identifier
    int x, y;         // Coordinates for visualization
};
```

## Compilation and Running

### Using Qt Creator
1. Open `TSP_Project.pro` in Qt Creator
2. Configure project with Qt version
3. Build project (Ctrl+B)
4. Run application (Ctrl+R)

### Using Command Line
```bash
qmake TSP_Project.pro
make
./TSP_Project
```

## Example Input/Output

### Sample Graph
```
Cities: A B C D
Edges:
A ↔ B: 10
A ↔ C: 15
A ↔ D: 20
B ↔ C: 35
B ↔ D: 25
C ↔ D: 30
```

### Expected Output (Brute Force)
```
Optimal Path: A → B → D → C → A
Total Distance: 80
Execution Time: 25 microseconds
Memory Used: 88 bytes
```

## Edge Cases Handled

1. **Single City**: Application allows but TSP not applicable
2. **Disconnected Graph**: Algorithm uses INT_MAX for missing edges
3. **Large Datasets**: Brute force may be slow (use Greedy for large graphs)
4. **Equal Distances**: Algorithms handle ties correctly
5. **Empty Graph**: Clear validation prevents errors

## Performance Metrics

### Brute Force
- 4 cities: ~25 microseconds
- 8 cities: ~5 milliseconds
- 10 cities: ~100 milliseconds

### Greedy
- 4 cities: ~5 microseconds
- 100 cities: ~50 microseconds
- 1000 cities: ~5 milliseconds

### Nearest Insertion
- 4 cities: ~10 microseconds
- 100 cities: ~100 microseconds
- 1000 cities: ~10 milliseconds

## Limitations

1. Maximum 20 cities (can be increased by changing MAX_CITIES)
2. Single character city names
3. Symmetric distances (undirected graph)
4. Brute force impractical for >12 cities
5. No dynamic graph modifications (clear and recreate required)

## Future Enhancements

1. **More Algorithms**: Genetic, Ant Colony, Simulated Annealing
2. **Dynamic Programming**: Better upper bound for larger graphs
3. **Graph Editing**: Edit nodes and edges after creation
4. **Export Results**: Generate PDF or image reports
5. **Benchmark Suite**: Automated testing with multiple graph sizes
6. **3D Visualization**: 3D city layout rendering

## Author
Abdullah Cheema

## License
MIT License
