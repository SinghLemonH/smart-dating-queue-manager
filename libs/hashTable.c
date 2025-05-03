#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"


User* hashTable[TABLE_SIZE] = { NULL };

unsigned int hash(const char* uid) {
    unsigned int hash = 0;
    while (*uid) {
        hash = (hash * 31) + (*uid++);
    }
    return hash % TABLE_SIZE;
}

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