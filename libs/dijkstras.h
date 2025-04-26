#ifndef DIJKSTRAS_H
#define DIJKSTRAS_H

#define MAX_LOCATIONS 15
#define INF 99999

// ฟังก์ชันสำหรับโหลดกราฟจากไฟล์ CSV
void loadGraphFromCSV(const char* filename);

// ฟังก์ชันสำหรับคำนวณระยะทางที่สั้นที่สุดด้วย Dijkstra's Algorithm
int dijkstra(int start, int end, int* path);

// ฟังก์ชันสำหรับคำนวณตารางความสัมพันธ์
void calculateSchedule(const char* userLocation, const char* uid);

// ฟังก์ชันสำหรับแสดงตารางนัดหมาย
void viewSchedule(const char* uid);

void editSchedule();

#endif // DIJKSTRAS_H