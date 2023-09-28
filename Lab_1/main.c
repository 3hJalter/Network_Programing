#include <stdio.h>
#include <stdlib.h>
#include "lib/StudentScheduleManagement.h"
#include "lib/DynamicList.h"
#include "lib/Path.h"

char strInput[20];
User *user;
DynamicList *studentRs;
DynamicList *courseSs;

void onLogin();

void onInitScreen();

void onExitScreen();

void displayLoginScreen();

void displayMainMenuScreen();

int main() {
    // INIT DATA
    studentRs = GetStudentRegistrationList(studentRegisterPath);
    courseSs = GetCourseList(courseSchedulePath);
    // MAIN
#if defined(__linux__) || defined(__unix__)
    printf("This is Linux, Enter anything to open system\n");
#endif
    displayLoginScreen();
    // OUT MAIN
    freeCourseList(courseSs);
    freeStudentRegistrationList(studentRs);
    freeUser(user);
    return 0;
}

void onInitScreen() {
#ifdef _WIN32
    fflush(stdin);
#else
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        // Read and discard characters until a newline or EOF is encountered
    }
#endif
    printf("--------------------\n");
}

void onExitScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
    printf("Exit screen\n");
}

void displayLoginScreen() {
    long int choice;
    do {
        onInitScreen();
        printf("----- Welcome to Student Schedule Management -----\n");
        printf("| 1. Login                                       |\n");
        printf("| 2. Exit Screen                                 |\n");
        printf("--------------------------------------------------\n");
        printf("--> Enter your choice: ");
        if(fgets(strInput, sizeof(strInput), stdin) != NULL) {
            choice = strtol(strInput, NULL, 10);
        }
        switch (choice) {
            case 1:
                onLogin();
                break;
            case 2:
                onExitScreen();
                break;
            default:
                printf("--> Error: %s is invalid choice\n", strInput);
                break;
        }
    } while (choice != 2);
}

void onLogin() {
    char username[20];
    char password[20];
    fflush(stdin);
    printf("- Enter username: ");
    scanf("%s", username);
    printf("- Enter password: ");
    scanf("%s", password);
    user = UserLogin(username, password, userAccountPath);
    if (user != NULL) {
        displayMainMenuScreen();
    }
}

void displayMainMenuScreen() {
    long int choice;
    DynamicList *classIdList = GetClassIdListByUserId(studentRs, user->username);
    do {
        onInitScreen();
        printf("------------------- Main screen ------------------\n");
        printf("| Welcome %-38s |\n", user->username);
        printf("| 1. Read schedule                               |\n");
        printf("| 2. Exit screen                                 |\n");
        printf("--------------------------------------------------\n");
        printf("--> Enter your choice: ");
        if(fgets(strInput, sizeof(strInput), stdin) != NULL) {
            choice = strtol(strInput, NULL, 10);
        }
        switch (choice) {
            case 1:
                printf("- Enter day of week: ");
                char dayOfWeekStr[20];
                scanf("%s", dayOfWeekStr);
                if (strcmp(dayOfWeekStr, "All") != 0 && strcmp(dayOfWeekStr, "all") != 0) {
                    DayOfWeek dayOfWeek;
                    dayOfWeek = ConvertDayStringToEnum(dayOfWeekStr);
                    PrintCourseListByDayStudyAndClassIdList(courseSs, dayOfWeek, classIdList);
                    break;
                } else PrintCourseListByClassIdList(courseSs, classIdList);
                break;
            case 2:
                freeDynamicList(classIdList);
                classIdList = NULL;
                onExitScreen();
#if defined(__linux__) || defined(__unix__)
                printf("Enter anything to return continue\n");
#endif
                break;
            default:
                printf("--> Error: Invalid choice\n");
                break;
        }
    } while (choice != 2);
}