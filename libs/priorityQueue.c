#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priorityQueue.h"
#include "../include/dataManage.h" // สำหรับ getDistance()

Partner* head = NULL; // หัวของ Priority Queue
Partner* tail = NULL; // ท้ายของ Priority Queue

// Add a partner to the priority queue
void addPartner(const char* userLocation, const char* partnerName, const char* partnerLocation, int relationshipScore, const char* uid) {
    int distance = getDistance(userLocation, partnerLocation);
    if (distance == -1) {
        printf("Error: Could not find distance for location %s to %s\n", userLocation, partnerLocation);
        return;
    }

    Partner* newPartner = (Partner*)malloc(sizeof(Partner));
    strcpy(newPartner->name, partnerName);
    strcpy(newPartner->place, partnerLocation);
    newPartner->relationshipScore = relationshipScore;
    newPartner->distance = distance;
    newPartner->next = NULL;

    // Insert into Priority Queue (sorted by relationshipScore and distance)
    if (head == NULL || 
        (relationshipScore > head->relationshipScore) || 
        (relationshipScore == head->relationshipScore && distance < head->distance)) {
        newPartner->next = head;
        head = newPartner;

        // อัปเดต tail หากเป็น Partner ตัวแรก
        if (tail == NULL) {
            tail = newPartner;
        }
    } else {
        Partner* current = head;
        while (current->next != NULL && 
              (current->next->relationshipScore > relationshipScore || 
              (current->next->relationshipScore == relationshipScore && current->next->distance <= distance))) {
            current = current->next;
        }
        newPartner->next = current->next;
        current->next = newPartner;

        // อัปเดต tail หากเพิ่ม Partner ที่ท้ายลิสต์
        if (newPartner->next == NULL) {
            tail = newPartner;
        }
    }

    printf("Partner %s added successfully!\n", partnerName);

    // Save Priority Queue to partner_UID.csv
    savePartnersToFile(uid);
}

// Delete a partner from the priority queue
void deletePartner(const char* partnerName) {
    if (head == NULL) {
        printf("Priority Queue is empty.\n");
        return;
    }

    Partner* current = head;
    Partner* prev = NULL;

    while (current != NULL) {
        if (strcmp(current->name, partnerName) == 0) {
            if (prev == NULL) {
                // ลบ Partner ตัวแรก
                head = current->next;

                // อัปเดต tail หากลบ Partner ตัวสุดท้าย
                if (head == NULL) {
                    tail = NULL;
                }
            } else {
                // ลบ Partner ตรงกลางหรือท้าย
                prev->next = current->next;

                // อัปเดต tail หากลบ Partner ตัวสุดท้าย
                if (current == tail) {
                    tail = prev;
                }
            }
            free(current);
            printf("Partner %s deleted successfully!\n", partnerName);
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("Partner %s not found in the queue.\n", partnerName);
}

// View the partner status (Priority Queue)
void viewPartnerStatus() {
    if (head == NULL) {
        printf("No partners available.\n");
        return;
    }

    printf("\n=== Partner Status ===\n");
    Partner* current = head;
    while (current != NULL) {
        const char* level = getRelationshipLevel(current->relationshipScore); // แปลงคะแนนเป็นชื่อระดับ
        printf("Name: %s, Place: %s, Relationship Score: %d (%s), Distance: %d km\n",
               current->name, current->place, current->relationshipScore, level, current->distance);
        current = current->next;
    }
}

// Free the memory used by the Priority Queue
void freePriorityQueue() {
    Partner* current = head;
    while (current != NULL) {
        Partner* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    tail = NULL; // อัปเดต tail ให้เป็น NULL
    printf("Priority Queue cleared.\n");
}

// Function to get relationship level name
const char* getRelationshipLevel(int score) {
    switch (score) {
        case 1:
            return "Acquaintance"; // คนรู้จัก
        case 2:
            return "Casual Friend"; // เพื่อนทั่วไป
        case 3:
            return "Close Friend"; // เพื่อนสนิท
        case 4:
            return "Best Friend"; // เพื่อนที่ดีที่สุด
        case 5:
            return "Soulmate"; // คู่ชีวิต
        default:
            return "Unknown"; // กรณีคะแนนไม่อยู่ในช่วง 1-5
    }
}