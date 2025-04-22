#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/hashTable.h"
#include "../include/dataManage.h"
#include "../libs/priorityQueue.h"

// Function to check if the file exists
int fileExists(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file) {
        fclose(file);
        return 1; // File exists
    }
    return 0; // File does not exist
}

// Function to create the file if it does not exist
void createFileIfNotExists(const char* filename) {
    if (!fileExists(filename)) {
        printf("File %s does not exist. Creating a new one...\n", filename);
        FILE* file = fopen(filename, "w");
        if (!file) {
            perror("Error creating file");
            exit(1); // Exit the program if file creation fails
        }
        fclose(file);
        printf("File %s created successfully.\n", filename);
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
        perror("Error opening partner file");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char name[50], place[50];
        int relationshipScore, distance;

        // อ่านข้อมูลจากไฟล์ CSV
        if (sscanf(line, "%[^,],%[^,],%d,%d", name, place, &relationshipScore, &distance) == 4) {
            // เพิ่ม Partner เข้า Priority Queue
            Partner* newPartner = (Partner*)malloc(sizeof(Partner));
            strcpy(newPartner->name, name);
            strcpy(newPartner->place, place);
            newPartner->relationshipScore = relationshipScore;
            newPartner->distance = distance;
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
    printf("Partner data loaded from %s\n", filename);
}

// Save Priority Queue to partner_UID.csv
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
        fprintf(file, "%s,%s,%d,%d\n", current->name, current->place, current->relationshipScore, current->distance);
        current = current->next;
    }

    fclose(file);
    printf("Partner data saved to %s\n", filename);
}

// Function to get distance from distance.csv
int getDistance(const char* from, const char* to) {
    FILE* file = fopen("data/distance.csv", "r");
    if (!file) {
        perror("Error opening distance.csv");
        return -1;
    }

    char line[256];
    char place1[50], place2[50];
    int distance;

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^,],%[^,],%d", place1, place2, &distance);
        if (strcmp(from, place1) == 0 && strcmp(to, place2) == 0) {
            fclose(file);
            return distance;
        }
    }

    fclose(file);
    return -1; // Return -1 if no matching place is found
}