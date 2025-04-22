#ifndef DIJKSTRAS_H
#define DIJKSTRAS_H

#define MAX_LOCATIONS 100
#define INF 999999

// Function declarations
void loadGraphFromFile(const char* filename, int graph[MAX_LOCATIONS][MAX_LOCATIONS], char locations[MAX_LOCATIONS][50], int* locationCount);
int findShortestPath(int graph[MAX_LOCATIONS][MAX_LOCATIONS], int locationCount, const char* source, const char* destination, char locations[MAX_LOCATIONS][50]);

#endif // DIJKSTRAS_H