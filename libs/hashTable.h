#ifndef HASHTABLE_H
#define HASHTABLE_H

#define TABLE_SIZE 1000

typedef struct User {
    char uid[10];
    char username[50];
    char password[50];
    struct User* next;
} User;

extern User* hashTable[TABLE_SIZE];

unsigned int hash(const char* uid);
void insertUser(const char* uid, const char* username, const char* password);
User* searchUser(const char* uid);
void freeHashTable();

#endif // HASHTABLE_H