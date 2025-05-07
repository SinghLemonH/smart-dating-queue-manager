#include <stdio.h>
#include "../include/auth.h"
#include "../include/dataManage.h"

int main() {
    const char* userFile = "data/users.csv";

    //ensure the user file exists
    createFileIfNotExists(userFile, NULL);

    //load users from file
    loadUsersFromFile(userFile);

    //display the login menu
    loginMenu(userFile);

    return 0;
}