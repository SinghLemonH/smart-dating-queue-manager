#ifndef AUTH_H
#define AUTH_H

void loginMenu(const char* filename); 
void mainMenu(const char* filename, const char* uid); 
void login(const char* filename); 
void registerUser(const char* filename);
void logout();
void clearScreen();

#endif // AUTH_H