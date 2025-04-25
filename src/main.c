#include <stdio.h>
#include "../include/auth.h"
#include "../include/dataManage.h"

int main() {
    const char* userFile = "data/users.csv";

    // Ensure the user file exists
    createFileIfNotExists(userFile, NULL);

    // Load users from file
    loadUsersFromFile(userFile);

    // Display the login menu
    loginMenu(userFile);

    return 0;
}