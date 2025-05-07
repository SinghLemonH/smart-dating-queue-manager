#ifndef DIJKSTRAS_H
#define DIJKSTRAS_H

#define MAX_LOCATIONS 15
#define INF 99999

int dijkstra(int start, int end, int* path);

float calculateScore(int relationshipScore, int distance);
void calculateSchedule(const char* userLocation, const char* uid);


void viewSchedule(const char* uid);
void editSchedule(const char* uid);

void displayUserLocation();

void displayPartnerLocation(const char* userLocation);

#endif