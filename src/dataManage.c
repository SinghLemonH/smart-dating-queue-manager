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
        return 1; //have file
    }
    return 0; // have not file
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
            exit(1);
        }
        fclose(file);
        printf("File %s created successfully.\n", filename);
    } else {
        fclose(file);
    }
}
//Convert csv into hashTable eiei
void loadUsersFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char uid[10], username[50], password[50];
        sscanf(line, "%[^,],%[^,],%s", uid, username, password); 
        insertUser(uid, username, password);
    }

    fclose(file);
    printf("Users loaded successfully from %s.\n", filename);
}

// Convert Hashtable into csv eiei
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
        char name[50], place[50];
        int relationshipScore;

        if (sscanf(line, "%[^,],%d,%[^,\n]", name, &relationshipScore, place) == 3) {
            Partner* newPartner = (Partner*)malloc(sizeof(Partner));
            strcpy(newPartner->name, name);
            newPartner->relationshipScore = relationshipScore;
            strcpy(newPartner->place, place); // โหลดข้อมูล place
            newPartner->next = NULL;

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
        fprintf(file, "%s,%d,0,0\n", current->name, current->relationshipScore);
        current = current->next;
    }

    fclose(file);
    printf("Partner data saved to %s\n", filename);
}

void updatePartnerLocation(const char* uid, const char* partnerName, const char* userLocation, const char* partnerLocation) {
    char filename[100];
    sprintf(filename, "data/partners/partner_%s.csv", uid);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    char tempFilename[100];
    sprintf(tempFilename, "data/partners/temp_%s.csv", uid);
    FILE* tempFile = fopen(tempFilename, "w");
    if (!tempFile) {
        printf("Error: Could not create temporary file.\n");
        fclose(file);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char name[50];
        int relationshipScore;
        char userLoc[50], partnerLoc[50];

        if (sscanf(line, "%[^,],%d,%[^,],%[^,\n]", name, &relationshipScore, userLoc, partnerLoc) == 4) {
            if (strcmp(name, partnerName) == 0) {
                // uppdate location
                fprintf(tempFile, "%s,%d,%s,%s\n", name, relationshipScore, userLocation, partnerLocation);
            } else {
                // write origin data
                fprintf(tempFile, "%s,%d,%s,%s\n", name, relationshipScore, userLoc, partnerLoc);
            }
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(filename);
    rename(tempFilename, filename);

    printf("Updated location for Partner: %s\n", partnerName);
}

void saveScheduleToFile(const char* uid) {
    char filename[100];
    sprintf(filename, "data/schedule/sch_%s.csv", uid);

    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Error: Could not open file %s for writing.\n", filename);
        return;
    }

    Partner* current = head;
    while (current != NULL) {
        fprintf(file, "%s,%s\n", current->name, current->time); // บันทึกชื่อ Partner และวันที่
        current = current->next;
    }

    fclose(file);
    printf("Schedule saved to %s successfully.\n", filename);
}

void loadScheduleFromFile(const char* uid) {
    char filename[100];
    sprintf(filename, "data/schedule/sch_%s.csv", uid);

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("No existing schedule found for UID: %s.\n", uid);
        return;
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        char name[50], time[20];
        if (sscanf(line, "%[^,],%s", name, time) == 2) {
            Partner* current = head;
            int exists = 0;
            while (current != NULL) {
                if (strcmp(current->name, name) == 0) {
                    exists = 1;
                    break;
                }
                current = current->next;
            }

            if (exists) {
                strcpy(current->time, time);
                continue;
            }

            Partner* newPartner = (Partner*)malloc(sizeof(Partner));
            if (!newPartner) {
                printf("Error: Memory allocation failed.\n");
                fclose(file);
                return;
            }

            strcpy(newPartner->name, name);
            strcpy(newPartner->time, time);
            newPartner->next = NULL;

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
    printf("Schedule loaded from %s successfully.\n", filename);
}