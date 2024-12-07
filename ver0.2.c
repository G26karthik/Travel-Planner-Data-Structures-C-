#include <stdio.h>
#include <string.h>
#include <limits.h>

#define MAX_NODES 100
#define INF INT_MAX
#define NAME_LENGTH 50

typedef struct {
    char name[NAME_LENGTH];
} Node;

Node nodes[MAX_NODES];  // Array to store node names

// Function to find the index of a node by name
int getNodeIndex(char name[]) {
    for (int i = 0; i < MAX_NODES; i++) {
        if (strcmp(nodes[i].name, name) == 0) {
            return i;
        }
    }
    return -1;  // Return -1 if node is not found
}

// Function to read input data (nodes, edges, and weights)
void readInputData(FILE *file, int *n, int *e, int graph[MAX_NODES][MAX_NODES]) {
    fscanf(file, "%d", n);  // Read number of nodes
    for (int i = 0; i < *n; i++) {
        fscanf(file, "%s", nodes[i].name);  // Read node names
    }

    fscanf(file, "%d", e);  // Read number of edges
    for (int i = 0; i < *e; i++) {
        char srcName[NAME_LENGTH], destName[NAME_LENGTH];
        int weight;

        fscanf(file, "%s %s %d", srcName, destName, &weight);

        int u = getNodeIndex(srcName);
        int v = getNodeIndex(destName);

        if (u == -1 || v == -1) {
            printf("Error: Invalid node name in edge.\n");
            continue;
        }

        graph[u][v] = weight;
        graph[v][u] = weight;  // For undirected graph; remove this line for directed graphs
    }
}

// Function to display the adjacency matrix (visual representation)
void displayAdjacencyMatrix(int graph[MAX_NODES][MAX_NODES], int n) {
    int maxNameLength = 0;

    // Determine the maximum name length for alignment
    for (int i = 0; i < n; i++) {
        if (strlen(nodes[i].name) > maxNameLength) {
            maxNameLength = strlen(nodes[i].name);
        }
    }

    printf("\nAdjacency Matrix Representation:\n");
    printf("%-*s", maxNameLength + 2, " ");  // Print empty space for the top-left corner
    for (int i = 0; i < n; i++) {
        printf("%-*s", maxNameLength + 2, nodes[i].name);  // Print column headers
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%-*s", maxNameLength + 2, nodes[i].name);  // Print row headers
        for (int j = 0; j < n; j++) {
            if (graph[i][j] == 0 || graph[i][j] == INF) {
                printf("%-*s", maxNameLength + 2, "INF");
            } else {
                printf("%-*d", maxNameLength + 2, graph[i][j]);
            }
        }
        printf("\n");
    }
}

// Function to find the node with the minimum distance that hasn't been processed
int findMinDistance(int dist[], int processed[], int n) {
    int min = INF, min_index;
    for (int i = 0; i < n; i++) {
        if (!processed[i] && dist[i] <= min) {
            min = dist[i];
            min_index = i;
        }
    }
    return min_index;
}

// Function implementing Dijkstra's algorithm
void dijkstra(int graph[MAX_NODES][MAX_NODES], int src, int n) {
    int dist[MAX_NODES];       // Holds shortest distance from src to each node
    int processed[MAX_NODES];   // Marks nodes as processed

    // Initialize distances to INF and processed array to 0
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        processed[i] = 0;
    }
    dist[src] = 0;  // Distance to source is 0

    // Find shortest path for all nodes
    for (int count = 0; count < n - 1; count++) {
        int u = findMinDistance(dist, processed, n);
        processed[u] = 1;  // Mark node u as processed

        // Update distance value of adjacent nodes of the picked node
        for (int v = 0; v < n; v++) {
            if (!processed[v] && graph[u][v] && dist[u] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    // Output the result
    printf("\nNode\t\tDistance from Source\n");
    for (int i = 0; i < n; i++) {
        printf("%-15s", nodes[i].name);  // Align node names
        if (dist[i] == INF) {
            printf("No Path\n");
        } else {
            printf("%d\n", dist[i]);
        }
    }
}

// Function to list all direct connections from the source node
void listDirectConnections(int graph[MAX_NODES][MAX_NODES], int src, int n) {
    printf("\nDirect connections from %s:\n", nodes[src].name);
    for (int i = 0; i < n; i++) {
        if (graph[src][i] != 0 && graph[src][i] != INF) {
            printf(" - %-15s (Weight: %d)\n", nodes[i].name, graph[src][i]);
        }
    }
}

int main() {
    int n, e;
    int graph[MAX_NODES][MAX_NODES] = {0};
    char filename[100], srcName[NAME_LENGTH];
    FILE *file;

    // Open the file for reading
    printf("Enter the file name: ");
    scanf("%s", filename);
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", filename);
        return 1;
    }

    // Read data from the input file and build the graph
    readInputData(file, &n, &e, graph);
    fclose(file);

    // Display the adjacency matrix for visual representation
    displayAdjacencyMatrix(graph, n);

    // Let the user choose the source node
    printf("\nEnter the source node: ");
    scanf("%s", srcName);
    int src = getNodeIndex(srcName);

    if (src == -1) {
        printf("Error: Invalid source node name.\n");
        return 1;
    }

    // List all direct connections from the source node
    listDirectConnections(graph, src, n);

    // Calculate shortest paths using Dijkstra's algorithm
    dijkstra(graph, src, n);

    return 0;
}
