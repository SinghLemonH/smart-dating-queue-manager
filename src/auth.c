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
        printf("\n+-----------------------------------+\n");
        printf("|             LOGIN MENU            |\n");
        printf("+-----------------------------------+\n");
        printf("| 1. Login                          |\n");
        printf("| 2. Register                       |\n");
        printf("| 3. Exit                           |\n");
        printf("+-----------------------------------+\n");
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
                printf("\nExiting the system. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 3);
}

void mainMenu(const char* filename, const char* uid) {
    int choice;
    do {
        printf("\n+-----------------------------------+\n");
        printf("|           MAIN MENU               |\n");
        printf("+-----------------------------------+\n");
        printf("| 1. View Partners Status           |\n");
        printf("| 2. Add Partner                    |\n");
        printf("| 3. Delete Partner                 |\n");
        printf("| 4. Schedule a date                |\n");
        printf("| 5. View Schedule                  |\n");
        printf("| 6. Logout                         |\n");
        printf("+-----------------------------------+\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nViewing partner status for UID: %s\n", uid);
                viewPartnerStatus(); // เรียกฟังก์ชันแสดงตารางนัดหมาย
                break;
            case 2: {
                printf("\nAdding partner for UID: %s\n", uid);
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
                printf("\nDeleting partner for UID: %s\n", uid);
                char partnerName[50];
                printf("Enter Partner Name to Delete: ");
                scanf("%s", partnerName);

                deletePartner(partnerName, uid); // ลบ Partner
                break;
            }
            case 4:
                printf("\nScheduling a date...\n");
                break;
            case 5:
                printf("\nViewing schedule...\n");
                break;
            case 6:
                printf("\nExiting user menu. Goodbye!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
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