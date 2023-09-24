#include <stdio.h>
#include <stdlib.h>
#include "lib/StudentScheduleManagement.h"
#include "lib/DynamicList.h"

#define studentRegisterPath "D:\\Code\\Workspace\\C\\Network_Program\\Lab_1\\textData\\student_registration.txt"
#define courseSchedulePath "D:\\Code\\Workspace\\C\\Network_Program\\Lab_1\\textData\\course_schedule.txt"
#define userAccountPath "D:\\Code\\Workspace\\C\\Network_Program\\Lab_1\\textData\\User-account.txt"

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
    displayLoginScreen();
    // OUT MAIN
    freeCourseList(courseSs);
    freeStudentRegistrationList(studentRs);
    freeUser(user);
    return 0;
}

void onInitScreen() {
    fflush(stdin);
    printf("--------------------\n");
}

void onExitScreen() {
    system("cls");
    printf("Exit screen\n");
}

void displayLoginScreen() {
    int choice;
    do {
        onInitScreen();
        printf("----- Welcome to Student Schedule Management -----\n");
        printf("| 1. Login                                       |\n");
        printf("| 2. Exit Screen                                 |\n");
        printf("--------------------------------------------------\n");
        printf("--> Enter your choice: ");
        choice = strtol(gets(strInput), NULL, 10);
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
    int choice;
    DynamicList *classIdList = GetStudentRegistrationListByUserId(studentRs, user->username);
    do {
        onInitScreen();
        printf("------------------- Main screen ------------------\n");
        printf("| Welcome %-38s |\n", user->username);
        printf("| 1. Read schedule                               |\n");
        printf("| 2. Exit screen                                 |\n");
        printf("--------------------------------------------------\n");
        printf("--> Enter your choice: ");
        choice = strtol(gets(strInput), NULL, 10);
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
                break;
            default:
                printf("--> Error: Invalid choice\n");
                break;
        }
    } while (choice != 2);
}