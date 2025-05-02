#include <stdio.h>
#include <string.h>
#include <time.h>

#include "dijkstras.h"
#include "priorityQueue.h"

#include "../include/dataManage.h"

#define INF 99999

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

    // ดึงวันที่ปัจจุบันและเริ่มต้นจากวันพรุ่งนี้
    time_t t = time(NULL);
    struct tm* currentDate = localtime(&t);
    currentDate->tm_mday += 1; // เพิ่ม 1 วัน
    mktime(currentDate); // ปรับวันที่ให้ถูกต้อง (เช่น ข้ามเดือน)

    while (current != NULL) {
        printf("Enter location for %s (or type 'skip' to skip this partner): ", current->name);
        char partnerLocation[50];
        scanf("%s", partnerLocation);

        // ตรวจสอบว่าผู้ใช้พิมพ์ "skip" หรือ "ไม่นัด"
        if (strcmp(partnerLocation, "skip") == 0 || strcmp(partnerLocation, "ไม่นัด") == 0) {
            // รีเซ็ตเวลานัดหมายของ Partner นี้
            strcpy(current->time, "N/A");
            prev = current;
            current = current->next;
            continue;
        }

        // ตรวจสอบว่าสถานที่ที่กรอกถูกต้องหรือไม่
        int partnerIndex = -1;
        for (int i = 0; i < locationCount; i++) {
            if (strcmp(locations[i], partnerLocation) == 0) {
                partnerIndex = i;
                break;
            }
        }

        if (partnerIndex == -1) {
            printf("Invalid location: %s. Skipping this partner.\n", partnerLocation);
            strcpy(current->time, "N/A"); // รีเซ็ตเวลานัดหมาย
            prev = current;
            current = current->next;
            continue;
        }

        // อัปเดตสถานที่ในไฟล์ CSV
        updatePartnerLocation(uid, current->name, userLocation, partnerLocation);

        // คำนวณระยะทาง
        int path[MAX_LOCATIONS];
        int distance = dijkstra(userIndex, partnerIndex, path);

        if (distance != INF) {
            // คำนวณจำนวนวันที่ต้องใช้ในการเดินทาง
            int travelDays = (distance / 500) + 1; // สมมติว่าเดินทางได้ 500 กม./วัน
            currentDate->tm_mday += travelDays; // เพิ่มจำนวนวันตามระยะทาง
            mktime(currentDate); // ปรับวันที่ให้ถูกต้อง (เช่น ข้ามเดือน)

            // บันทึกวันที่ในรูปแบบ dd/mm/yyyy
            sprintf(current->time, "%02d/%02d/%04d", currentDate->tm_mday, currentDate->tm_mon + 1, currentDate->tm_year + 1900);
        } else {
            strcpy(current->time, "N/A"); // หากไม่มีเส้นทาง
        }

        prev = current;
        current = current->next;
    }

    // บันทึกตารางนัดหมายลงไฟล์ CSV
    saveScheduleToFile(uid);
}

void viewSchedule(const char* uid) {
    if (head == NULL) {
        printf("No appointments available.\n");
        return;
    }

    printf("\n+-----------------------------------+\n");
    printf("| No. | Partner       | Date        |\n");
    printf("+-----------------------------------+\n");

    int index = 1;
    Partner* current = head;
    while (current != NULL) {
        printf("| %-3d | %-12s | %-10s |\n", index++, current->name, current->time);
        current = current->next;
    }

    printf("+-----------------------------------+\n");

    editSchedule(uid);
}

void editSchedule(const char* uid) {
    char choice;
    printf("Are you okay with this schedule? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        printf("Schedule confirmed.\n");
        return; // ออกจากฟังก์ชัน
    }

    if (choice == 'n' || choice == 'N') {
        int partnerIndex;
        char newDate[11]; // รูปแบบ dd/mm/yyyy
        int day, month, year;

        printf("Enter the number of the partner you want to edit: ");
        scanf("%d", &partnerIndex);

        Partner* current = head;
        int index = 1;

        // ค้นหา Partner ที่ต้องการแก้ไข
        while (current != NULL && index < partnerIndex) {
            current = current->next;
            index++;
        }

        if (current == NULL) {
            printf("Invalid partner number.\n");
            return;
        }

        // รับวันที่ใหม่จากผู้ใช้
        while (1) {
            printf("Enter the new date for %s (dd/mm/yyyy): ", current->name);
            scanf("%s", newDate);

            // ตรวจสอบรูปแบบวันที่
            if (sscanf(newDate, "%2d/%2d/%4d", &day, &month, &year) == 3 &&
                day >= 1 && day <= 31 &&
                month >= 1 && month <= 12 &&
                year >= 1900 && year <= 2100) {
                break;
            } else {
                printf("Invalid date format. Please try again.\n");
            }
        }

        // อัปเดตวันที่ในโครงสร้างข้อมูล
        strcpy(current->time, newDate);

        // บันทึกการเปลี่ยนแปลงลงไฟล์ CSV
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

    // ค้นหาดัชนีของสถานที่ผู้ใช้
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

    // คำนวณระยะทางที่สั้นที่สุดจาก userLocation ไปยังทุกสถานที่
    int dist[MAX_LOCATIONS];
    int path[MAX_LOCATIONS];
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        dist[i] = INF;
    }
    dist[userIndex] = 0; // ระยะทางจากตัวเองไปตัวเองคือ 0

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

    // แสดงตารางระยะทาง
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