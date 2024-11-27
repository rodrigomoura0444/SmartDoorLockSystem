#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define max attempts and password length
#define MAX_ATTEMPTS 3
#define PASSWORD_LENGTH 20
#define LOG_FILE "data/access_log.txt"

// Function declarations
void setPassword();
int authenticate();
void logAttempt(const char *status);
void menu();

int main() {
    printf("Welcome to the Smart Door Lock System!\n");
    menu();
    return 0;
}

// Menu for user actions
void menu() {
    int choice, attempts = 0;

    while (1) {
        printf("\n--- MENU ---\n");
        printf("1. Set Password\n");
        printf("2. Authenticate\n");
        printf("3. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            setPassword();
            break;
        case 2:
            if (authenticate()) {
                printf("Access Granted. Welcome!\n");
                logAttempt("Success");
            } else {
                attempts++;
                logAttempt("Failure");
                if (attempts >= MAX_ATTEMPTS) {
                    printf("Too many failed attempts! Security Alert triggered!\n");
                    exit(1);
                } else {
                    printf("Access Denied. Try Again.\n");
                }
            }
            break;
        case 3:
            printf("Exiting the system. Goodbye!\n");
            exit(0);
        default:
            printf("Invalid choice! Please try again.\n");
        }
    }
}

// Function to set a new password
void setPassword() {
    FILE *file = fopen("data/password.txt", "w");
    if (!file) {
        printf("Error opening file!\n");
        return;
    }

    char password[PASSWORD_LENGTH];
    printf("Enter new password: ");
    scanf("%s", password);

    fprintf(file, "%s", password);
    fclose(file);

    printf("Password updated successfully.\n");
}

// Function to authenticate the user
int authenticate() {
    FILE *file = fopen("data/password.txt", "r");
    if (!file) {
        printf("Password not set! Please set a password first.\n");
        return 0;
    }

    char storedPassword[PASSWORD_LENGTH], enteredPassword[PASSWORD_LENGTH];
    fscanf(file, "%s", storedPassword);
    fclose(file);

    printf("Enter password: ");
    scanf("%s", enteredPassword);

    if (strcmp(storedPassword, enteredPassword) == 0)
        return 1;
    else
        return 0;
}

// Function to log access attempts
void logAttempt(const char *status) {
    FILE *logFile = fopen(LOG_FILE, "a");
    if (!logFile) {
        printf("Error opening log file!\n");
        return;
    }

    fprintf(logFile, "Attempt: %s\n", status);
    fclose(logFile);
}
