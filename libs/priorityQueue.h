#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

// Partner structure
typedef struct Partner {
    char name[50];          // ชื่อของ Partner
    int relationshipScore;  // คะแนนความสัมพันธ์
    struct Partner* next;   // ชี้ไปยัง Partner ถัดไปใน Priority Queue
} Partner;

// Declare head and tail as external variables
extern Partner* head; // หัวของ Priority Queue
extern Partner* tail; // ท้ายของ Priority Queue

// Priority Queue functions
void addPartner(const char* partnerName, int relationshipScore, const char* uid);
void deletePartner(const char* partnerName, const char* uid);
void viewPartnerStatus(const char* uid);
void freePriorityQueue();

const char* getRelationshipLevel(int score);

#endif // PRIORITYQUEUE_H