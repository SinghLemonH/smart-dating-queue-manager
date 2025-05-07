#include <stdio.h>
#include <string.h>
#include <time.h>

#include "dijkstras.h"
#include "priorityQueue.h"

#include "../include/dataManage.h"

int graph[MAX_LOCATIONS][MAX_LOCATIONS] = {
    {0, 42, 62, INF, 52, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF}, // BU
    {42, 0, 28, INF, 34, 15, INF, INF, INF, INF, INF, INF, INF, INF, 62},  // CU
    {62, 28, 0, 31, INF, INF, INF, 935, INF, INF, INF, INF, INF, 36, 38},  // MU
    {INF, INF, 31, 0, INF, 17, INF, 947, 691, INF, 18, INF, 451, 23, INF}, // SWU
    {52, 34, INF, INF, 0, 45, INF, INF, INF, INF, INF, INF, INF, 34, INF}, // KMILT
    {INF, 15, INF, 17, 45, 0, 24, INF, INF, INF, INF, INF, INF, INF, INF}, // KMUTT
    {INF, INF, INF, INF, INF, 24, 0, 1442, INF, INF, INF, INF, 442, INF, INF}, // KMUTNB
    {INF, INF, 935, 947, INF, INF, 1442, 0, 1629, INF, INF, INF, 1395, INF, INF}, // PSU
    {INF, INF, INF, 691, INF, INF, INF, 1629, 0, 732, 705, INF, INF, 679, INF}, // CMU
    {INF, INF, INF, INF, INF, INF, INF, INF, 732, 0, INF, 84, INF, 63, INF}, // ABAC
    {INF, INF, INF, 18, INF, INF, INF, INF, 705, INF, 0, INF, INF, INF, 72}, // RU
    {INF, INF, INF, INF, INF, INF, INF, INF, INF, 84, INF, 0, 415, 30, INF}, // TU
    {INF, INF, INF, 451, INF, INF, 442, 1395, INF, INF, INF, 415, 0, 438, INF}, // KKU
    {INF, INF, 36, 23, 34, INF, INF, INF, 679, 63, INF, 30, 438, 0, INF}, // KU
    {INF, 62, 38, INF, INF, INF, INF, INF, INF, INF, 72, INF, INF, INF, 0}  // SU
};

char locations[MAX_LOCATIONS][10] = {
    "BU", "CU", "MU", "SWU", "KMILT", "KMUTT", "KMUTNB", "PSU", "CMU", "ABAC", "RU", "TU", "KKU", "KU", "SU"
};

int locationCount = 15; 

int dijkstra(int start, int end, int* path) {
    int dist[MAX_LOCATIONS], visited[MAX_LOCATIONS], prev[MAX_LOCATIONS];
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        dist[i] = INF;
        visited[i] = 0;
        prev[i] = -1;
    }
    dist[start] = 0;

    for (int i = 0; i < locationCount - 1; i++) {
        int minDist = INF, u = -1;
        for (int j = 0; j < locationCount; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < locationCount; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
                prev[v] = u;
            }
        }
    }

    if (dist[end] == INF) {
        return INF;
    }

    int current = end, pathIndex = 0;
    while (current != -1) {
        path[pathIndex++] = current;
        current = prev[current];
    }

    return dist[end];
}

// float calculateScore(int relationshipScore, int distance) {
//     if (distance == INF) {
//         return -INF;
//     }
//     return (0.7 * (relationshipScore * 10)) - (0.3 * distance);
// }

void calculateSchedule(const char* userLocation, const char* uid) {
    int userIndex = -1;

    for (int i = 0; i < locationCount; i++) {
        if (strcmp(locations[i], userLocation) == 0) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) {
        printf("Invalid user location: %s\n", userLocation);
        printf("Available locations:\n");
        for (int i = 0; i < locationCount; i++) {
            printf("- %s\n", locations[i]);
        }
        return;
    }

    Partner* current = head;
    Partner* prev = NULL;

    // pull today and start tomorrow naja
    time_t t = time(NULL);
    struct tm* currentDate = localtime(&t);
    currentDate->tm_mday += 1; // add 1 day
    mktime(currentDate); // handle chaged mount naja

    while (current != NULL) {
        printf("Enter location for %s (or type 'skip' to skip this partner): ", current->name);
        char partnerLocation[50];
        scanf("%s", partnerLocation);

        if (strcmp(partnerLocation, "skip") == 0 || strcmp(partnerLocation, "Skip") == 0) {
            strcpy(current->time, "N/A");
            current->distance = INF; // no dist >:(
            prev = current;
            current = current->next;
            continue;
        }

        // check location naja
        int partnerIndex = -1;
        for (int i = 0; i < locationCount; i++) {
            if (strcmp(locations[i], partnerLocation) == 0) {
                partnerIndex = i;
                break;
            }
        }

        if (partnerIndex == -1) {
            printf("Invalid location: %s. Skipping this partner.\n", partnerLocation);
            strcpy(current->time, "N/A");
            current->distance = INF;
            prev = current;
            current = current->next;
            continue;
        }

        updatePartnerLocation(uid, current->name, userLocation, partnerLocation);

        int path[MAX_LOCATIONS];
        int distance = dijkstra(userIndex, partnerIndex, path);

        if (distance != INF) {
            current->distance = distance;
            // float score = calculateScore(current->relationshipScore, distance);
            // printf("Score for %s: %.2f\n", current->name, score);

            // to cal date naja
            int travelDays = (distance / 300) + 1; // 300km per one
            currentDate->tm_mday += travelDays; // add day from distance
            mktime(currentDate); // to handle about changed mount

            //set format of date naja
            sprintf(current->time, "%02d/%02d/%04d", currentDate->tm_mday, currentDate->tm_mon + 1, currentDate->tm_year + 1900);
        } else {
            current->distance = INF;
            strcpy(current->time, "N/A"); // no line no location
        }

        prev = current;
        current = current->next;
    }

    
    saveScheduleToFile(uid);
}

void viewSchedule(const char* uid) {
    if (head == NULL) {
        printf("No appointments available.\n");
        return;
    }

    printf("\n+-------------------------------------------+\n");
    printf("| No. | Partner       | Date                |\n");
    printf("+-------------------------------------------+\n");

    int index = 1;
    Partner* current = head;
    while (current != NULL) {
        printf("| %-3d | %-12s | %-20s |\n", index++, current->name, current->time);
        current = current->next;
    }

    printf("+-------------------------------------------+\n");

    editSchedule(uid);
}

void editSchedule(const char* uid) {
    char choice;
    printf("Are you okay with this schedule? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        printf("Schedule confirmed.\n");
        return;
    }

    if (choice == 'n' || choice == 'N') {
        int partnerIndex;
        char newDate[11];
        int day, month, year;

        printf("Enter the number of the partner you want to edit: ");
        scanf("%d", &partnerIndex);

        Partner* current = head;
        int index = 1;

        while (current != NULL && index < partnerIndex) {
            current = current->next;
            index++;
        }

        if (current == NULL) {
            printf("Invalid partner number.\n");
            return;
        }

        while (1) {
            printf("Enter the new date for %s (dd/mm/yyyy): ", current->name);
            scanf("%s", newDate);

            // chack new date format
            if (sscanf(newDate, "%2d/%2d/%4d", &day, &month, &year) == 3 &&
                day >= 1 && day <= 31 &&
                month >= 1 && month <= 12 &&
                year >= 1900 && year <= 2100) {
                    
                struct tm input = { .tm_mday = day, .tm_mon = month-1, .tm_year = year-1900 }; //check past date ;k
                if (difftime(mktime(&input), time(NULL)) >= 0) break;
                else printf("Date is in the past. Try again.\n");
                break;
            } else {
                printf("Invalid date format. Please try again.\n");
            }
        }

        strcpy(current->time, newDate);

        saveScheduleToFile(uid);

        printf("Schedule updated successfully.\n");
    } else {
        printf("Invalid choice. Please enter 'y' or 'n'.\n");
    }
}

void displayUserLocation() {
    printf("\n+-------------------+\n");
    printf("| Available Locations |\n");
    printf("+-------------------+\n");
    for (int i = 0; i < locationCount; i++) {
        printf("| %-17s |\n", locations[i]);
    }
    printf("+-------------------+\n");
}

void displayPartnerLocation(const char* userLocation) {
    int userIndex = -1;

    for (int i = 0; i < locationCount; i++) {
        if (strcmp(locations[i], userLocation) == 0) {
            userIndex = i;
            break;
        }
    }

    if (userIndex == -1) {
        printf("Invalid user location: %s\n", userLocation);
        return;
    }

    // cal distance to all location 
    int dist[MAX_LOCATIONS];
    int path[MAX_LOCATIONS];
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        dist[i] = INF;
    }
    dist[userIndex] = 0; // distance itself = 0

    int visited[MAX_LOCATIONS] = {0};
    for (int i = 0; i < locationCount - 1; i++) {
        int minDist = INF, u = -1;
        for (int j = 0; j < locationCount; j++) {
            if (!visited[j] && dist[j] < minDist) {
                minDist = dist[j];
                u = j;
            }
        }

        if (u == -1) break;
        visited[u] = 1;

        for (int v = 0; v < locationCount; v++) {
            if (!visited[v] && graph[u][v] != INF && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    printf("\n+-------------------+-------------------+\n");
    printf("| Location          | Distance (km)    |\n");
    printf("+-------------------+-------------------+\n");
    for (int i = 0; i < locationCount; i++) {
        if (dist[i] == INF) {
            printf("| %-17s | %-17s |\n", locations[i], "Unreachable");
        } else {
            printf("| %-17s | %-17d |\n", locations[i], dist[i]);
        }
    }
    printf("+-------------------+-------------------+\n");
}