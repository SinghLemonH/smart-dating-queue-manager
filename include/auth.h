#ifndef AUTH_H
#define AUTH_H

// Function declarations
void loginMenu(const char* filename); // Display the login menu
void mainMenu(const char* filename, const char* uid); // Display the main menu for a logged-in user
void login(const char* filename); // Handle user login
void registerUser(const char* filename, int year); // Handle user registration

#endif // AUTH_H