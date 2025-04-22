#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"

// Hash table array
User* hashTable[TABLE_SIZE] = { NULL };

// Hash function using UID
unsigned int hash(const char* uid) {
    unsigned int hash = 0;
    while (*uid) {
        hash = (hash * 31) + *uid++; // คูณค่า hash ด้วย 31 และบวกค่าของตัวอักษร
    }
    return hash % TABLE_SIZE; // นำผลลัพธ์ไป mod ด้วยขนาดของตารางแฮช
}

// Insert user into hash table
void insertUser(const char* uid, const char* username, const char* password) {
    unsigned int index = hash(uid);
    User* newUser = (User*)malloc(sizeof(User));
    if (!newUser) {
        printf("Error: Memory allocation failed.\n");
        return;
    }
    strcpy(newUser->uid, uid);
    strcpy(newUser->username, username);
    strcpy(newUser->password, password);
    newUser->next = hashTable[index];
    hashTable[index] = newUser;
}

// Search user in hash table
User* searchUser(const char* uid) {
    unsigned int index = hash(uid);
    User* current = hashTable[index];
    while (current) {
        if (strcmp(current->uid, uid) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Free memory used by the hash table
void freeHashTable() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        User* current = hashTable[i];
        while (current) {
            User* temp = current;
            current = current->next;
            free(temp);
        }
        hashTable[i] = NULL;
    }
}