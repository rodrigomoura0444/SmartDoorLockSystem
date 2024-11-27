#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MAX_ATTEMPTS 3
#define PASSWORD_LENGTH 20
#define LOG_FILE "data/access_log.txt"

// Allowed access hours
#define ALLOWED_START_HOUR 8
#define ALLOWED_END_HOUR 20

// Max failed attempts before lockout
#define MAX_FAILED_ATTEMPTS 3
int failedAttempts = 0;  // Track failed attempts

void setPassword();
int authenticate();
void logAttempt(const char *status);
void menu();
int isAccessAllowed();
void changePassword();
int faceRecognitionAuthenticate();

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
        printf("2. Authenticate with Password\n");
        printf("3. Authenticate with Face Recognition\n");  // New option for Face Recognition
        printf("4. Change Password\n");
        printf("5. Exit\n");
        printf("Choose an option: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            setPassword();
            break;
        case 2:
            if (!isAccessAllowed()) {
                printf("Access is only allowed between %d:00 and %d:00.\n", ALLOWED_START_HOUR, ALLOWED_END_HOUR);
                logAttempt("Access Denied - Out of Hours");
                break;
            }
            if (authenticate()) {
                printf("Access Granted. Welcome!\n");
                logAttempt("Success");
                failedAttempts = 0;  // Reset failed attempts after success
            } else {
                failedAttempts++;
                logAttempt("Failure");
                if (failedAttempts >= MAX_FAILED_ATTEMPTS) {
                    printf("Too many failed attempts! Locking system for 10 seconds.\n");
                    sleep(10);  // Lockout for 10 seconds
                    failedAttempts = 0;  // Reset failed attempts after lockout
                } else {
                    printf("Access Denied. Try Again.\n");
                }
            }
            break;
        case 3:
            if (!isAccessAllowed()) {
                printf("Access is only allowed between %d:00 and %d:00.\n", ALLOWED_START_HOUR, ALLOWED_END_HOUR);
                logAttempt("Access Denied - Out of Hours");
                break;
            }
            if (faceRecognitionAuthenticate()) {
                printf("Access Granted via Face Recognition. Welcome!\n");
                logAttempt("Success");
            } else {
                logAttempt("Failure - Face Recognition");
                printf("Access Denied. Face not recognized.\n");
            }
            break;
        case 4:
            changePassword();  // Call changePassword function
            break;
        case 5:
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

// Function to authenticate the user with password
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

// Function to check if access is allowed based on the current time
int isAccessAllowed() {
    time_t now = time(NULL);
    struct tm *currentTime = localtime(&now);

    int currentHour = currentTime->tm_hour;
    if (currentHour >= ALLOWED_START_HOUR && currentHour < ALLOWED_END_HOUR) {
        return 1;
    }
    return 0;
}

// Function to change the password
void changePassword() {
    if (authenticate()) {
        setPassword();  // Reuse the setPassword function
        printf("Password successfully changed.\n");
    } else {
        printf("Authentication failed. Cannot change password.\n");
    }
}
