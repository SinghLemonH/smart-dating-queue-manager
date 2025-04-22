#ifndef DATAMANAGE_H
#define DATAMANAGE_H

// Function declarations
int fileExists(const char* filename);
void createFileIfNotExists(const char* filename);

void loadUsersFromFile(const char* filename);
void saveUsersToFile(const char* filename);

void loadPartnersFromFile(const char* uid);
void savePartnersToFile(const char* uid);

int getDistance(const char* from, const char* to); // Add this line

#endif // DATAMANAGE_H