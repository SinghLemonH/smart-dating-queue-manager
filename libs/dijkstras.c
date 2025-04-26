#include <stdio.h>
#include <string.h>
#include "dijkstras.h"
#include "priorityQueue.h"

#include "../include/dataManage.h"

#define INF 99999

// กำหนดข้อมูลกราฟโดยตรงจากไฟล์ distance.csv
int graph[MAX_LOCATIONS][MAX_LOCATIONS] = {
    // BU, CU, MU, SWU, KMILT, KMUTT, KMUTNB, PSU, CMU, ABAC, RU, TU, KKU, KU, SU
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

// ชื่อสถานที่
char locations[MAX_LOCATIONS][10] = {
    "BU", "CU", "MU", "SWU", "KMILT", "KMUTT", "KMUTNB", "PSU", "CMU", "ABAC", "RU", "TU", "KKU", "KU", "SU"
};

int locationCount = 15; // จำนวนสถานที่ทั้งหมด

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
        return INF; // No path found
    }

    // Reconstruct path
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
    int currentHour = 8; // เริ่มเวลานัดหมายที่ 08:00
    while (current != NULL) {
        int partnerIndex = -1;
        for (int i = 0; i < locationCount; i++) {
            if (strcmp(locations[i], current->place) == 0) {
                partnerIndex = i;
                break;
            }
        }

        if (partnerIndex != -1) {
            int path[MAX_LOCATIONS];
            int distance = dijkstra(userIndex, partnerIndex, path);
            if (distance != INF) {
                int travelTime = distance / 50; // คำนวณเวลาการเดินทาง (1 ชั่วโมงต่อ 50 กิโลเมตร)
                int startHour = currentHour + 1 + travelTime; // เว้น 1 ชั่วโมงก่อนเริ่ม และเพิ่มเวลาการเดินทาง
                int endHour = startHour + 1; // เวลานัดหมาย 1 ชั่วโมง
                sprintf(current->time, "%02d:00-%02d:00", startHour, endHour);
                currentHour = endHour; // อัปเดตเวลาสำหรับ Partner ถัดไป
            } else {
                strcpy(current->time, "N/A"); // หากไม่มีเส้นทาง
            }
        }

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

    printf("\n+-------------------------------+\n");
    printf("| No. | Partner       | Time       |\n");
    printf("+-------------------------------+\n");

    int index = 1;
    Partner* current = head;
    while (current != NULL) {
        printf("| %-3d | %-12s | %-10s |\n", index++, current->name, current->time);
        current = current->next;
    }

    printf("+-------------------------------+\n");

    // ถามผู้ใช้ว่าต้องการแก้ไขเวลาหรือไม่
    char choice;
    printf("Are you okay with this schedule? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'n' || choice == 'N') {
        editSchedule();
    }
}

void editSchedule() {
    int partnerNo;
    char newTime[20];

    printf("Enter the number of the partner you want to edit: ");
    scanf("%d", &partnerNo);

    Partner* current = head;
    int index = 1;
    while (current != NULL && index < partnerNo) {
        current = current->next;
        index++;
    }

    if (current == NULL) {
        printf("Invalid partner number.\n");
        return;
    }

    printf("Current time for %s: %s\n", current->name, current->time);
    printf("Enter the new time for %s (e.g., 10:00-11:00): ", current->name);
    scanf("%s", newTime);

    // อัปเดตเวลาของ Partner ที่เลือก
    strcpy(current->time, newTime);

    // ตรวจสอบและปรับเวลาของ Partner อื่น ๆ หากเวลาทับกัน
    Partner* other = head;
    while (other != NULL) {
        if (other != current) {
            int currentStart, currentEnd, otherStart, otherEnd;
            sscanf(current->time, "%d:00-%d:00", &currentStart, &currentEnd);
            sscanf(other->time, "%d:00-%d:00", &otherStart, &otherEnd);

            // ตรวจสอบว่าทับกันหรือไม่
            if ((currentStart < otherEnd && currentEnd > otherStart)) {
                // เลื่อนเวลาของ Partner อื่นออกไป
                otherStart = currentEnd + 1; // เริ่มหลังจากเวลาสิ้นสุดของ Partner ปัจจุบัน
                otherEnd = otherStart + 1;  // เวลานัดหมาย 1 ชั่วโมง
                sprintf(other->time, "%02d:00-%02d:00", otherStart, otherEnd);

                // อัปเดต currentEnd เพื่อให้ Partner ถัดไปไม่ทับกัน
                currentEnd = otherEnd;
            }
        }
        other = other->next;
    }

    printf("Schedule updated successfully for %s.\n", current->name);
}