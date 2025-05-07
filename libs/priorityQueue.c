#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "priorityQueue.h"
#include "../include/dataManage.h"
#include "dijkstras.h"

Partner* head = NULL;
Partner* tail = NULL;

void addPartner(const char* partnerName, int relationshipScore, const char* uid) {
    Partner* newPartner = (Partner*)malloc(sizeof(Partner));
    if (!newPartner) {
        printf("Error: Memory allocation failed.\n");
        return;
    }

    strcpy(newPartner->name, partnerName);
    newPartner->relationshipScore = relationshipScore;
    newPartner->next = NULL;

    // add Partner sort by relationship !!! :D XD ;p
    if (head == NULL || relationshipScore > head->relationshipScore) {
        newPartner->next = head;
        head = newPartner;

        if (tail == NULL) {
            tail = newPartner;
        }
    } else {
        Partner* current = head;
        while (current->next != NULL && current->next->relationshipScore >= relationshipScore) {
            current = current->next;
        }
        newPartner->next = current->next;
        current->next = newPartner;

        if (newPartner->next == NULL) {
            tail = newPartner;
        }
    }

    printf("Partner %s added successfully with relationship score %d.\n", partnerName, relationshipScore);

    savePartnersToFile(uid);
}

void deletePartner(const char* partnerName, const char* uid) {
    if (head == NULL) {
        printf("Priority Queue is empty.\n");
        return;
    }

    Partner* current = head;
    Partner* prev = NULL;
    Partner* target = NULL; 
    Partner* targetPrev = NULL; 

    //Serach Partner Start from score low to high 
    while (current != NULL) {
        if (strcmp(current->name, partnerName) == 0) {
            target = current;
            targetPrev = prev;
        }
        prev = current;
        current = current->next;
    }

    if (target == NULL) {
        printf("Partner %s not found in the queue.\n", partnerName);
        return;
    }

    if (targetPrev == NULL) {
        head = target->next;

        if (head == NULL) {
            tail = NULL;
        }
    } else {
        targetPrev->next = target->next;

        if (target == tail) { // udate tail after del last queue na
            tail = targetPrev;
        }
    }

    free(target);
    printf("Partner %s deleted successfully!\n", partnerName);

    savePartnersToFile(uid);
}

/*!!!!!!This func have the problem!!!!!!!
1)after you run program and logout and login again it's will view csv old uid last login
2)after view it's delet data of partners detail in csv */

void viewPartnerStatus(const char* uid) {

    createFileIfNotExists("data/partners/partner_%s.csv", uid);

    freePriorityQueue();

    loadPartnersFromFile(uid);

    if (head == NULL) {
        printf("No partners available.\n");
        return;
    }

    printf("\n+--------------------------------------+\n");
    printf("|         Partner Status               |\n");
    printf("+--------------------------------------+\n");
    printf("| Name                 | Status        |\n");
    printf("+--------------------------------------+\n");

    Partner* current = head;
    while (current != NULL) {
        // ใช้ getRelationshipLevel() เพื่อแปลงคะแนนเป็นสถานะ
        const char* status = getRelationshipLevel(current->relationshipScore);
        printf("| %-20s | %-13s |\n", current->name, status);
        current = current->next;
    }

    printf("+--------------------------------------+\n");
}

void freePriorityQueue() {
    Partner* current = head;
    while (current != NULL) {
        Partner* temp = current;
        current = current->next;
        free(temp);
    }
    head = NULL;
    tail = NULL; 
    printf("Priority Queue cleared.\n");
}

const char* getRelationshipLevel(int score) {
    switch (score) {
        case 1:
            return "Acquaintance"; //คนรู้จัก
        case 2:
            return "Casual Friend"; //เพื่อนทั่วไป
        case 3:
            return "Close Friend"; //เพื่อนสนิท
        case 4:
            return "Best Friend"; //เพื่อนที่ดีที่สุด
        case 5:
            return "Soulmate"; //คู่ชีวิต
        default:
            return "Unknown"; //กรณีคะแนนไม่อยู่ในช่วง 1-5
    }
}

void displayRelation() {
    printf("\n+-------------------+-------------------+\n");
    printf("| Relationship Score | Status            |\n");
    printf("+-------------------+-------------------+\n");
    printf("| 1                 | Acquaintance      |\n");
    printf("| 2                 | Casual Friend     |\n");
    printf("| 3                 | Close Friend      |\n");
    printf("| 4                 | Best Friend       |\n");
    printf("| 5                 | Soulmate          |\n");
    printf("+-------------------+-------------------+\n");
}