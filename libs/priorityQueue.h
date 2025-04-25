#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

// Partner structure
typedef struct Partner {
    char name[50];          // ชื่อของ Partner
    char place[50];         // สถานที่ (ดึงจาก distance.csv)
    int relationshipScore;  // คะแนนความสัมพันธ์
    int distance;           // ระยะทางจากไฟล์ distance.csv
    struct Partner* next;   // ชี้ไปยัง Partner ถัดไปใน Priority Queue
} Partner;

// Declare head and tail as external variables
extern Partner* head; // หัวของ Priority Queue
extern Partner* tail; // ท้ายของ Priority Queue

// Priority Queue functions
void addPartner(const char* userLocation, const char* partnerName, const char* partnerLocation, int relationshipScore, const char* uid);
void deletePartner(const char* partnerName, const char* uid);
void viewPartnerStatus();
void freePriorityQueue();

const char* getRelationshipLevel(int score);

#endif // PRIORITYQUEUE_H