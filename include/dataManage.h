#ifndef DATAMANAGE_H
#define DATAMANAGE_H

// Function declarations
int fileExists(const char* format, const char* uid);
void createFileIfNotExists(const char* format, const char* uid);

void loadUsersFromFile(const char* filename);
void saveUsersToFile(const char* filename);

void loadPartnersFromFile(const char* uid);
void savePartnersToFile(const char* uid);

void updatePartnerLocation(const char* uid, const char* partnerName, const char* userLocation, const char* partnerLocation);

void saveScheduleToFile(const char* uid);
void loadScheduleFromFile(const char* uid);

#endif // DATAMANAGE_H