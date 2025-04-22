#include <stdio.h>
#include <string.h>
#include "dijkstras.h"

// Load graph from distance.csv
void loadGraphFromFile(const char* filename, int graph[MAX_LOCATIONS][MAX_LOCATIONS], char locations[MAX_LOCATIONS][50], int* locationCount) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening distance.csv");
        return;
    }

    char line[256];
    char place1[50], place2[50];
    int distance;
    *locationCount = 0;

    // Initialize graph with INF
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            graph[i][j] = (i == j) ? 0 : INF;
        }
    }

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%d", place1, place2, &distance);

        // Add locations to the list
        int index1 = -1, index2 = -1;
        for (int i = 0; i < *locationCount; i++) {
            if (strcmp(locations[i], place1) == 0) index1 = i;
            if (strcmp(locations[i], place2) == 0) index2 = i;
        }
        if (index1 == -1) {
            strcpy(locations[*locationCount], place1);
            index1 = (*locationCount)++;
        }
        if (index2 == -1) {
            strcpy(locations[*locationCount], place2);
            index2 = (*locationCount)++;
        }

        // Update graph
        graph[index1][index2] = distance;
        graph[index2][index1] = distance; // Assuming undirected graph
    }

    fclose(file);
}

// Find shortest path using Dijkstra's Algorithm
int findShortestPath(int graph[MAX_LOCATIONS][MAX_LOCATIONS], int locationCount, const char* source, const char* destination, char locations[MAX_LOCATIONS][50]) {
    int dist[MAX_LOCATIONS], visited[MAX_LOCATIONS];
    int prev[MAX_LOCATIONS];
    for (int i = 0; i < locationCount; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }

    // Find source index
    int sourceIndex = -1, destinationIndex = -1;
    for (int i = 0; i < locationCount; i++) {
        if (strcmp(locations[i], source) == 0) sourceIndex = i;
        if (strcmp(locations[i], destination) == 0) destinationIndex = i;
    }
    if (sourceIndex == -1 || destinationIndex == -1) {
        printf("Error: Source or destination not found.\n");
        return -1;
    }

    dist[sourceIndex] = 0;

    for (int count = 0; count < locationCount - 1; count++) {
        // Find the minimum distance vertex
        int minDist = INF, minIndex = -1;
        for (int i = 0; i < locationCount; i++) {
            if (!visited[i] && dist[i] < minDist) {
                minDist = dist[i];
                minIndex = i;
            }
        }

        if (minIndex == -1) break; // No reachable vertex
        visited[minIndex] = 1;

        // Update distances
        for (int i = 0; i < locationCount; i++) {
            if (!visited[i] && graph[minIndex][i] != INF && dist[minIndex] + graph[minIndex][i] < dist[i]) {
                dist[i] = dist[minIndex] + graph[minIndex][i];
                prev[i] = minIndex;
            }
        }
    }

    // Print shortest path
    printf("Shortest path from %s to %s: %d km\n", source, destination, dist[destinationIndex]);
    return dist[destinationIndex];
}