#include <stdio.h>
#include "../include/auth.h"
#include "../include/dataManage.h"

int main() {
    const char* filename = "data/users.csv";

    // Ensure the file exists
    createFileIfNotExists(filename);

    // Load users from file
    loadUsersFromFile(filename);

    // Display the login menu
    loginMenu(filename);

    return 0;
}