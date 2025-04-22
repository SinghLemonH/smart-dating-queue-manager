#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libs/hashTable.h"
#include "../include/dataManage.h"
#include "../include/auth.h"
#include "../libs/priorityQueue.h"
#include "../libs/dijkstras.h"

// Function to display the login menu
void loginMenu(const char* filename) {
    int choice;
    do {
        printf("\n=== Login Menu ===\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                login(filename);
                break;
            case 2:
                registerUser(filename, 2025); // Example year for UID generation
                break;
            case 3:
                printf("Exiting the system. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

// Function to display the main menu
void mainMenu(const char* filename, const char* uid) {
    int choice;
    do {
        printf("\n=== Main Menu ===\n");
        printf("1. View Partners Status\n");
        printf("2. Add Partner\n");
        printf("3. Delete Partner\n");
        printf("4. Schedule a date\n");
        printf("5. View Schedule\n");
        printf("6. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Viewing partner status for UID: %s\n", uid);
                viewPartnerStatus(); // เรียกฟังก์ชันแสดงตารางนัดหมาย
                break;
            case 2: {
                printf("Adding partner for UID: %s\n", uid);
                char partnerName[50], partnerLocation[50];
                int relationshipScore;

                printf("Enter Partner Name: ");
                scanf("%s", partnerName);
                printf("Enter Partner Location: ");
                scanf("%s", partnerLocation);
                printf("Enter Relationship Score: ");
                scanf("%d", &relationshipScore);

                addPartner("KMUTT", partnerName, partnerLocation, relationshipScore, uid); // เพิ่ม Partner
                break;
            }
            case 3: {
                printf("Deleting partner for UID: %s\n", uid);
                char partnerName[50];
                printf("Enter Partner Name to Delete: ");
                scanf("%s", partnerName);

                deletePartner(partnerName); // ลบ Partner
                break;
            }
            case 4: {
                printf("Scheduling a date...\n");
                int graph[MAX_LOCATIONS][MAX_LOCATIONS];
                char locations[MAX_LOCATIONS][50];
                int locationCount;
            
                // Load graph from distance.csv
                loadGraphFromFile("data/distance.csv", graph, locations, &locationCount);
            
                char destination[50];
                printf("Enter destination: ");
                scanf("%s", destination);
            
                int distance = findShortestPath(graph, locationCount, "KMUTT", destination, locations);
                if (distance != -1) {
                    printf("Scheduled a date at %s (Distance: %d km)\n", destination, distance);
                }
                break;
            }
            case 5: {
                printf("Viewing schedule...\n");
            
                // ตรวจสอบว่ามี Partner หรือไม่
                if (head == NULL) {
                    printf("No partners available to schedule.\n");
                    break;
                }
            
                int graph[MAX_LOCATIONS][MAX_LOCATIONS];
                char locations[MAX_LOCATIONS][50];
                int locationCount;
            
                // Load graph from distance.csv
                loadGraphFromFile("data/distance.csv", graph, locations, &locationCount);
            
                Partner* current = head;
                while (current != NULL) {
                    // คำนวณระยะทางจาก KMUTT ไปยังสถานที่ของ Partner
                    int distance = findShortestPath(graph, locationCount, "KMUTT", current->place, locations);
                    if (distance != -1) {
                        printf("Name: %s, Place: %s, Distance: %d km, Relationship Score: %d\n",
                               current->name, current->place, distance, current->relationshipScore);
                    } else {
                        printf("Error: Could not calculate distance to %s\n", current->place);
                    }
                    current = current->next;
                }
                break;
            }
            case 6:
                printf("Exiting user menu. Goodbye!\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 6);
}

void login(const char* filename) {
    char username[50], password[50];
    printf("\n=== Login ===\n");
    printf("Enter Username: ");
    scanf("%s", username);
    printf("Enter Password: ");
    scanf("%s", password);

    // Search for the user by username
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        while (current) {
            if (strcmp(current->username, username) == 0 && strcmp(current->password, password) == 0) {
                printf("Login successful! Welcome, %s\n", current->username);

                // ตรวจสอบและโหลดข้อมูล Partner จากไฟล์
                char partnerFile[100];
                sprintf(partnerFile, "data/partners/partner_%s.csv", current->uid);

                // โหลดข้อมูล Partner เข้า Priority Queue
                loadPartnersFromFile(current->uid);

                // Proceed to main menu
                mainMenu(filename, current->uid);
                return;
            }
            current = current->next;
        }
    }

    printf("Invalid Username or Password. Please try again.\n");
}

void registerUser(const char* filename, int year) {
    char username[50], password[50], uid[10];
    printf("\n=== Register ===\n");
    printf("Enter Username: ");
    scanf("%s", username);

    // Check if the username already exists
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        while (current) {
            if (strcmp(current->username, username) == 0) {
                printf("Error: Username '%s' is already taken. Please try again.\n", username);
                return;
            }
            current = current->next;
        }
    }

    printf("Enter Password: ");
    scanf("%s", password);

    // Generate UID
    int sequence = 1;
    do {
        sprintf(uid, "%d%02d", year % 100, sequence++);
    } while (searchUser(uid));

    // Insert user into hash table
    insertUser(uid, username, password);

    // Save updated hash table to file
    saveUsersToFile(filename);

    printf("Registration successful! Your UID is %s\n", uid);
}