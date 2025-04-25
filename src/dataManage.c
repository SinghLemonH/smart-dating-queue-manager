#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/hashTable.h"
#include "../include/dataManage.h"
#include "../libs/priorityQueue.h"

int fileExists(const char* format, const char* uid) {
    char filename[100];
    sprintf(filename, format, uid);

    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // ไฟล์มีอยู่
    }
    return 0; // ไฟล์ไม่มีอยู่
}

void createFileIfNotExists(const char* format, const char* uid) {
    char filename[100];
    sprintf(filename, format, uid);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("File %s does not exist. Creating a new one...\n", filename);
        file = fopen(filename, "w");
        if (!file) {
            perror("Error creating file");
            exit(1); // Exit the program if file creation fails
        }
        fclose(file);
        printf("File %s created successfully.\n", filename);
    } else {
        fclose(file);
    }
}

// Function to load users from CSV file into the hash table
void loadUsersFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char uid[10], username[50], password[50];
        sscanf(line, "%[^,],%[^,],%s", uid, username, password); // Parse CSV line
        insertUser(uid, username, password); // Insert into hash table
    }

    fclose(file);
    printf("Users loaded successfully from %s.\n", filename);
}

// Function to save users from the hash table to a CSV file
void saveUsersToFile(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing\n", filename);
        return;
    }

    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        while (current) {
            fprintf(file, "%s,%s,%s\n", current->uid, current->username, current->password); // Write to CSV
            current = current->next;
        }
    }

    fclose(file);
    printf("Users saved successfully to %s.\n", filename);
}

void loadPartnersFromFile(const char* uid) {
    char filename[100];
    sprintf(filename, "data/partners/partner_%s.csv", uid);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No partner file found for UID: %s\n", uid);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char name[50];
        int relationshipScore;

        // อ่านข้อมูลจากไฟล์ CSV
        if (sscanf(line, "%[^,],%d", name, &relationshipScore) == 2) {
            Partner* newPartner = (Partner*)malloc(sizeof(Partner));
            strcpy(newPartner->name, name);
            newPartner->relationshipScore = relationshipScore;
            newPartner->next = NULL;

            // เพิ่ม Partner เข้า Priority Queue
            if (head == NULL) {
                head = newPartner;
                tail = newPartner;
            } else {
                tail->next = newPartner;
                tail = newPartner;
            }
        }
    }

    fclose(file);
    printf("Partners loaded successfully for UID: %s\n", uid);
}

void savePartnersToFile(const char* uid) {
    char filename[100];
    sprintf(filename, "data/partners/partner_%s.csv", uid);

    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening partner file");
        return;
    }

    Partner* current = head;
    while (current != NULL) {
        // บันทึกค่า userLocation และ partnerLocation เป็น 0,0
        fprintf(file, "%s,%d,0,0\n", current->name, current->relationshipScore);
        current = current->next;
    }

    fclose(file);
    printf("Partner data saved to %s\n", filename);
}
