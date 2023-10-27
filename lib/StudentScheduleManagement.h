//
// Created by Jalter on 9/21/2023.
//

#ifndef NETWORK_PROGRAM_SSM_H
#define NETWORK_PROGRAM_SSM_H

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdbool.h>
#include "DayOfWeek.h"
#include "DynamicList.h"

// Base function
void addWeekRange(DynamicList *weekStudy, int startWeek, int endWeek) {
    for (int week = startWeek; week <= endWeek; week++) {
        addToDynamicList(weekStudy, &week, sizeof(week));
    }
}

// Get a string from list DynamicList int type
// Example: "1,2,3,5,8,9,10,11,12,13,14,15"
// Return "1-3,5,8-15"
char *GetWeekStringFromList(DynamicList *weekStudy) {
    char *weekString = malloc(sizeof(char) * 100);
    char *weekRange = malloc(sizeof(char) * 20);
    weekString[0] = '\0';
    weekRange[0] = '\0';
    int startWeek = 0;
    int endWeek = 0;
    int isRange = 0;
    for (int i = 0; i < weekStudy->size; i++) {
        int week;
        memcpy(&week, (char *) weekStudy->data + (i * weekStudy->elementSize), weekStudy->elementSize);
        if (i == 0) {
            startWeek = week;
            endWeek = week;
        } else {
            if (week == endWeek + 1) {
                endWeek = week;
                isRange = 1;
            } else {
                if (isRange) {
                    sprintf(weekRange, "%d-%d,", startWeek, endWeek);
                    strcat(weekString, weekRange);
                    isRange = 0;
                } else {
                    sprintf(weekRange, "%d,", endWeek);
                    strcat(weekString, weekRange);
                }
                startWeek = week;
                endWeek = week;
            }
        }
    }
    if (isRange) {
        sprintf(weekRange, "%d-%d", startWeek, endWeek);
        strcat(weekString, weekRange);
    } else {
        sprintf(weekRange, "%d", endWeek);
        strcat(weekString, weekRange);
    }
    free(weekRange);
    return weekString;
}

ssize_t getLine(char **linePtr, size_t *n, FILE *stream) {
    if (linePtr == NULL || n == NULL || stream == NULL) {
        return -1; // Invalid arguments
    }

    ssize_t bytesRead = 0;
    size_t bufferSize = *n;

    // Check if linePtr is NULL or buffer size is too small
    if (*linePtr == NULL || bufferSize < 2) {
        bufferSize = 128; // Initial buffer size (adjust as needed)
        *linePtr = (char *) malloc(bufferSize);

        if (*linePtr == NULL) {
            return -1; // Memory allocation failed
        }

        *n = bufferSize;
    }

    int c;
    char *line = *linePtr;

    while ((c = fgetc(stream)) != EOF) {
        if (bytesRead >= (ssize_t) (bufferSize - 1)) {
            // Resize the buffer if needed
            bufferSize *= 2;
            char *newLine = (char *) realloc(line, bufferSize);

            if (newLine == NULL) {
                return -1; // Memory allocation failed during resizing
            }

            line = newLine;
            *linePtr = line;
            *n = bufferSize;
        }

        line[bytesRead++] = (char) c;

        if (c == '\n') {
            break; // End of line
        }
    }

    if (bytesRead == 0) {
        return -1; // No data read (EOF reached)
    }

    line[bytesRead] = '\0'; // Null-terminate the line

    return bytesRead;
}

typedef struct course_schedule {
    char classId[20];
    char courseId[20];
    char courseName[50];
    DayOfWeek dayStudy;
    int startAt;
    int endAt;
    DynamicList *weekStudy;
    char room[20];
} Course_S;

// Function to CreateCourse
Course_S *CreateCourse(char *classId, char *courseId, char *courseName, DayOfWeek dayStudy,
                       int startAt, int endAt, DynamicList *weekStudy, char *room) {
    Course_S *course = (Course_S *) malloc(sizeof(Course_S));
    strcpy(course->classId, classId);
    strcpy(course->courseId, courseId);
    strcpy(course->courseName, courseName);
    course->dayStudy = dayStudy;
    course->startAt = startAt;
    course->endAt = endAt;
    course->weekStudy = weekStudy;
    strcpy(course->room, room);
    return course;
}


void addCourseList(DynamicList *courseList, Course_S *course) {
    // Create a new copy of weekStudy of this course
    DynamicList *weekStudy = initializeList(sizeof(int));
    for (int i = 0; i < course->weekStudy->size; i++) {
        int week;
        memcpy(&week, (char *) course->weekStudy->data + (i * course->weekStudy->elementSize),
               course->weekStudy->elementSize);
        addToDynamicList(weekStudy, &week, sizeof(int));
    }
    // Create a new copy of this course
    Course_S *tCourse = CreateCourse(course->classId, course->courseId, course->courseName, course->dayStudy,
                                     course->startAt, course->endAt, weekStudy, course->room);
    addToDynamicList(courseList, tCourse, sizeof(Course_S));
    free(tCourse);
    tCourse = NULL;
}

void freeCourseList(DynamicList *courseList) {
    for (int i = 0; i < courseList->size; i++) {
        Course_S *course = (Course_S *) getElement(courseList, i);
        freeDynamicList(course->weekStudy);
    }
    freeDynamicList(courseList);
}

// Get a list of course from file path
DynamicList *GetCourseList(char *path) {
    FILE *fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Can not open file\n");
        return NULL;
    }
    // Memory for each line in file
    char *line = NULL;
    size_t line_length = 0;
    // Init course list
    DynamicList *courseList = initializeList(sizeof(Course_S));
    while (getLine(&line, &line_length, fp) != -1) {
        char classId[20];
        char courseId[20];
        char courseName[50];
        char theRest[100];
        if (sscanf(line, "%s %s %[^0-9] %s",
                   classId, courseId, courseName, theRest) == 4) {
//            printf("Read line: %s\n", line);
            int dayStudy, startAt, endAt;
            char *token = NULL;
            token = strtok(theRest, ",");
            dayStudy = token[0] - '0';
            startAt = 6 * ((token[1] - '0') - 1) + (token[2] - '0');
            token = strtok(NULL, ",");
            endAt = 6 * ((token[1] - '0') - 1) + (token[2] - '0');
            DynamicList *weekStudy = initializeList(sizeof(int));
            char *room = NULL;
            // For converting week-week format
            int startWeek, endWeek;
            while (token != NULL) {
                token = strtok(NULL, ",");
                if (token == NULL) break;
                if (sscanf(token, "%d-%d", &startWeek, &endWeek) == 2) {
                    addWeekRange(weekStudy, startWeek, endWeek);
                } else if (isdigit(token[0])) {
                    long int week = strtol(token, NULL, 10);
                    addToDynamicList(weekStudy, &week, sizeof(int));
                } else if (isalpha(token[0])) {
                    room = malloc(sizeof(char) * strlen(token));
                    strcpy(room, token);
                    room[strlen(room) - 1] = '\0';
                }
            }
            // Create a new Course_S struct and add it to List
            Course_S *tCourse = CreateCourse(classId, courseId, courseName, dayStudy, startAt, endAt, weekStudy, room);
            addToDynamicList(courseList, tCourse, sizeof(Course_S));
            free(tCourse);
            tCourse = NULL;
        } else {
            printf("Error parsing line: %s\n", line);
        }
    }
    fclose(fp);
    return courseList;
}

// Get a list of course that have the same dayStudy parameter and list of classId parameter from courseList
DynamicList *GetCourseListByDayStudyAndClassIdList(DynamicList *courseList, DayOfWeek dayStudy,
                                                   DynamicList *classIdList) {
    DynamicList *resultList = initializeList(sizeof(Course_S));
    for (int i = 0; i < courseList->size; i++) {
        Course_S *course = (Course_S *) getElement(courseList, i);
        if (course->dayStudy == dayStudy) {
            for (int j = 0; j < classIdList->size; j++) {
                char *classId = (char *) getElement(classIdList, j);
                if (strcmp(course->classId, classId) == 0) {
//                    addToDynamicList(resultList, course, sizeof(Course_S));
                    addCourseList(resultList, course);
                }
            }
        }
    }
    return resultList;
}

char* PrintCourseSchedule2(Course_S *course) {
    char *weekString = GetWeekStringFromList(course->weekStudy);
    int startAt = course->startAt;
    int endAt = course->endAt;
    if (startAt > 6) {
        startAt -= 6;
        endAt -= 6;
    }
    char *result = malloc(sizeof(char) * 1000);
    sprintf(result, "|%-8s|%-20s|%-10s|%-10s|%d-%-6d|%-21s|%-8s|\n", course->courseId, course->courseName,
            ConvertDayEnumToString(course->dayStudy), course->startAt <= 6 ? "Morning" : "Afternoon", startAt, endAt,
            weekString, course->room);
    free(weekString);
    weekString = NULL;
    return result;
}

// Function to Print Course
void PrintCourseSchedule(Course_S *course) {
    char *weekString = GetWeekStringFromList(course->weekStudy);
    int startAt = course->startAt;
    int endAt = course->endAt;
    if (startAt > 6) {
        startAt -= 6;
        endAt -= 6;
    }
    printf("|%-8s|%-20s|%-10s|%-10s|%d-%-6d|%-21s|%-8s|\n", course->courseId, course->courseName,
           ConvertDayEnumToString(course->dayStudy), course->startAt <= 6 ? "Morning" : "Afternoon", startAt, endAt,
           weekString, course->room);
    printf("|--------|--------------------|----------|----------|--------|---------------------|--------|\n");
    free(weekString);
    weekString = NULL;
}

char *ConvertCourseListToJson(DynamicList *courseList) {
    char *jsonString = malloc(sizeof(char) * 1000);
    char *jsonStringTemp = malloc(sizeof(char) * 1000);
    jsonString[0] = '\0';
    jsonStringTemp[0] = '\0';
    strcat(jsonString, "[");
    for (int i = 0; i < courseList->size; i++) {
        Course_S *course = (Course_S *) getElement(courseList, i);
        char *weekString = GetWeekStringFromList(course->weekStudy);
        int startAt = course->startAt;
        int endAt = course->endAt;
        if (startAt > 6) {
            startAt -= 6;
            endAt -= 6;
        }
        sprintf(jsonStringTemp,
                "{\"courseId\":\"%s\",\"courseName\":\"%s\",\"dayStudy\":\"%s\",\"startAt\":\"%d\",\"endAt\":\"%d\",\"weekStudy\":\"%s\",\"room\":\"%s\"},",
                course->courseId, course->courseName, ConvertDayEnumToString(course->dayStudy), startAt, endAt,
                weekString, course->room);
        strcat(jsonString, jsonStringTemp);
        free(weekString);
        weekString = NULL;
    }
    jsonString[strlen(jsonString) - 1] = '\0';
    strcat(jsonString, "]");
    free(jsonStringTemp);
    return jsonString;
}

DynamicList *ConvertJsonToCourseList(char *jsonString) {

    char *start = strchr(jsonString, '[');
    char *end = strrchr(jsonString, ']');

    if (start && end && start < end) {
        memmove(start, start + 1, end - start);
        memmove(end - 1, end + 1, strlen(end) + 1);
    }
    printf("Init JsonString: %s\n", jsonString);
    char *tokenString;
    char *jsonStringCopy = strdup(jsonString);
    char *startJsonString = jsonStringCopy;
    char *endJsonString = jsonStringCopy;
    DynamicList *courseList = initializeList(sizeof(Course_S));
    while (*endJsonString != '\0') {
        if (*endJsonString == '}' && (*(endJsonString + 1) == ',' || *(endJsonString + 1) == '\0')) {
            *endJsonString = '}';
            tokenString = strndup(startJsonString, endJsonString - startJsonString + 1);

            if (startJsonString != jsonStringCopy && *startJsonString == ',') {
                // If there's a leading comma, skip it
                tokenString++;
            }

            printf("Token string: %s\n", tokenString);
            char courseId[20];
            char courseName[50];
            char dayStudy[20];
            int startAt, endAt;
            char weekStudy[100];
            char room[20];
            if (sscanf(tokenString,
                       "{\"courseId\":\"%19[^\"]\",\"courseName\":\"%49[^\"]\",\"dayStudy\":\"%19[^\"]\",\"startAt\":\"%d\",\"endAt\":\"%d\",\"weekStudy\":\"%99[^\"]\",\"room\":\"%19[^\"]\"}",
                       courseId, courseName, dayStudy, &startAt, &endAt, weekStudy, room) == 7) {
                // Create a new Course_S struct and add it to List
                DynamicList *weekStudyList = initializeList(sizeof(int));
                int startWeek, endWeek;
                char *tokenWeek = NULL;
                tokenWeek = strtok(weekStudy, ",");
                while (tokenWeek != NULL) {
                    if (sscanf(tokenWeek, "%d-%d", &startWeek, &endWeek) == 2) {
                        addWeekRange(weekStudyList, startWeek, endWeek);
                    } else if (isdigit(tokenWeek[0])) {
                        long int week = strtol(tokenWeek, NULL, 10);
                        addToDynamicList(weekStudyList, &week, sizeof(int));
                    }
                    tokenWeek = strtok(NULL, ",");
                }
                Course_S *tCourse = CreateCourse("", courseId, courseName, ConvertDayStringToEnum(dayStudy), startAt,
                                                 endAt, weekStudyList, room);
                addToDynamicList(courseList, tCourse, sizeof(Course_S));
                free(tCourse);
                tCourse = NULL;
            } else {
                printf("Error parsing line: %s\n", tokenString);
            }
//            free(tokenString);
            startJsonString = endJsonString + 1;
        }
        endJsonString++;
    }
    printf("Complete\n");
    free(jsonStringCopy);
    return courseList;
}

char* PrintCourseListByDayAndStudentRegistrationList2(DynamicList *courseList, DayOfWeek dayStudy,
                                                      DynamicList *classIdList) {
    DynamicList *resultList = GetCourseListByDayStudyAndClassIdList(courseList, dayStudy, classIdList);
    if (dayStudy == INVALID) {
        printf("--> Error: Invalid Day\n");
        return "";
    }
    char *result = malloc(sizeof(char) * 1000);
    result[0] = '\0';
    sprintf(result, "---------------------------------------- Course List ----------------------------------------\n");
    sprintf(result, "%s|Code    |Course Name         |Week day  |AM/PM     |Period  |Week                 |Room    |\n", result);
    sprintf(result, "%s|--------|--------------------|----------|----------|--------|---------------------|--------|\n", result);
    for (int i = 0; i < resultList->size; i++) {
        Course_S *course = (Course_S *) getElement(resultList, i);
        char *courseSchedule = PrintCourseSchedule2(course);
        sprintf(result, "%s%s", result, courseSchedule);
        free(courseSchedule);
        courseSchedule = NULL;
    }
    freeCourseList(resultList);
    resultList = NULL;
    return result;
}

// Print a list of course that have the same dayStudy parameter and list of classId parameter from courseList
void PrintCourseListByDayStudyAndStudentRegistrationList(DynamicList *courseList, DayOfWeek dayStudy,
                                                         DynamicList *classIdList) {
    DynamicList *resultList = GetCourseListByDayStudyAndClassIdList(courseList, dayStudy, classIdList);
    if (dayStudy == INVALID) {
        printf("--> Error: Invalid Day\n");
        return;
    }
    printf("---------------------------------------- Course List ----------------------------------------\n");
    printf("|Code    |Course Name         |Week day  |AM/PM     |Period  |Week                 |Room    |\n");
    printf("|--------|--------------------|----------|----------|--------|---------------------|--------|\n");
    for (int i = 0; i < resultList->size; i++) {
        Course_S *course = (Course_S *) getElement(resultList, i);
        PrintCourseSchedule(course);
    }
    freeCourseList(resultList);
    resultList = NULL;
}

char* PrintCourseListByStudentRegistrationList2(DynamicList *courseList, DynamicList *classIdList) {
    typedef struct lesson {
        char room[20];
        int startAt;
        int endAt;
        DayOfWeek dayStudy;
    } Lesson;
    DynamicList *lessonList = initializeList(sizeof(Lesson));
    for (int i = 0; i < classIdList->size; i++) {
        char *classId = (char *) getElement(classIdList, i);
        for (int j = 0; j < courseList->size; j++) {
            Course_S *course = (Course_S *) getElement(courseList, j);
            if (strcmp(course->classId, classId) == 0) {
                // convert it to lesson
                Lesson *lesson = (Lesson *) malloc(sizeof(Lesson));
                strcpy(lesson->room, course->room);
                lesson->startAt = course->startAt;
                lesson->endAt = course->endAt;
                lesson->dayStudy = course->dayStudy;
                addToDynamicList(lessonList, lesson, sizeof(Lesson));
                free(lesson);
                lesson = NULL;
            }
        }
    }
    char *result = malloc(sizeof(char) * 1000);
    result[0] = '\0';
    char table[12][5][20];
    // Init table
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++) {
            strcpy(table[i][j], "");
        }
    }
    // Populate table
    for (int i = 0; i < lessonList->size; i++) {
        Lesson *lesson = (Lesson *) getElement(lessonList, i);
        for (int j = lesson->startAt - 1; j < lesson->endAt; j++) {
            strcpy(table[j][lesson->dayStudy - 2], lesson->room);
        }
    }
    // Print table
    sprintf(result, "=========================================================\n");
    sprintf(result, "%sPeriod | Monday | Tuesday | Wednesday | Thursday | Friday\n", result);
    sprintf(result, "%s---------------------------------------------------------\n", result);
    for (int i = 0; i < 12; i++) {
        sprintf(result, "%s%-6d |", result, i + 1);
        for (int j = 0; j < 5; j++) {
            sprintf(result, "%s %-7s |", result, table[i][j]);
        }
        sprintf(result, "%s\n", result);
    }
    sprintf(result, "%s=========================================================\n", result);
    freeDynamicList(lessonList);
    lessonList = NULL;
    return result;
}

// Print a list of course that have the same classId in list of classId parameter from courseList
void PrintCourseListByStudentRegistrationList(DynamicList *courseList, DynamicList *classIdList) {
    typedef struct lesson {
        char room[20];
        int startAt;
        int endAt;
        DayOfWeek dayStudy;
    } Lesson;
    DynamicList *lessonList = initializeList(sizeof(Lesson));
    for (int i = 0; i < classIdList->size; i++) {
        char *classId = (char *) getElement(classIdList, i);
        for (int j = 0; j < courseList->size; j++) {
            Course_S *course = (Course_S *) getElement(courseList, j);
            if (strcmp(course->classId, classId) == 0) {
                // convert it to lesson
                Lesson *lesson = (Lesson *) malloc(sizeof(Lesson));
                strcpy(lesson->room, course->room);
                lesson->startAt = course->startAt;
                lesson->endAt = course->endAt;
                lesson->dayStudy = course->dayStudy;
                addToDynamicList(lessonList, lesson, sizeof(Lesson));
                free(lesson);
                lesson = NULL;
            }
        }
    }
    char table[12][5][20];
    // Init table
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < 5; j++) {
            strcpy(table[i][j], "");
        }
    }
    // Populate table
    for (int i = 0; i < lessonList->size; i++) {
        Lesson *lesson = (Lesson *) getElement(lessonList, i);
        for (int j = lesson->startAt - 1; j < lesson->endAt; j++) {
            strcpy(table[j][lesson->dayStudy - 2], lesson->room);
        }
    }
    // Print table
    printf("=========================================================\n");
    printf("Period | Monday | Tuesday | Wednesday | Thursday | Friday\n");
    printf("---------------------------------------------------------\n");
    for (int i = 0; i < 12; i++) {
        printf("%-6d |", i + 1);
        for (int j = 0; j < 5; j++) {
            printf(" %-7s |", table[i][j]);
        }
        printf("\n");
    }
    printf("=========================================================\n");
    freeDynamicList(lessonList);
    lessonList = NULL;
}

typedef struct student_registration {
    char userId[20];
    char classId[20];
} Student_R;

Student_R *CreateStudentRegistration(char *userId, char *classId) {
    Student_R *studentRegistration = (Student_R *) malloc(sizeof(Student_R));
    strcpy(studentRegistration->userId, userId);
    strcpy(studentRegistration->classId, classId);
    return studentRegistration;
}

void PrintStudentRegistration(Student_R *studentRegistration) {
    printf("UserId: %s, ", studentRegistration->userId);
    printf("ClassId: %s\n", studentRegistration->classId);
}

// Get a list of student registration from file path
DynamicList *GetStudentRegistrationList(char *path) {
    FILE *fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Can not open file\n");
        return NULL;
    }
    // Memory for each line in file
    char *line = NULL;
    size_t line_length = 0;
    // Init student registration list
    DynamicList *studentRegistrationList = initializeList(sizeof(Student_R));
    while (getLine(&line, &line_length, fp) != -1) {
        char userId[20];
        char classId[20];
        if (sscanf(line, "%s %s", userId, classId) == 2) {
            // Create a new Student_R struct and add it to List
            Student_R *tStudentRegistration = CreateStudentRegistration(userId, classId);
            addToDynamicList(studentRegistrationList, tStudentRegistration, sizeof(Student_R));
            free(tStudentRegistration);
            tStudentRegistration = NULL;
        } else {
            printf("Error parsing line: %s\n", line);
        }
    }
    fclose(fp);
    return studentRegistrationList;
}

void freeStudentRegistrationList(DynamicList *studentRegistrationList) {
    freeDynamicList(studentRegistrationList);
}

DynamicList *GetClassIdListByUserId(DynamicList *studentRegistrationList, char *userId) {
    DynamicList *classIdList = initializeList(sizeof(char) * 20);
    for (int i = 0; i < studentRegistrationList->size; i++) {
        Student_R *studentRegistration = (Student_R *) getElement(studentRegistrationList, i);
        if (strcmp(studentRegistration->userId, userId) == 0) {
            addToDynamicList(classIdList, studentRegistration->classId, sizeof(char) * 20);
        }
    }
    return classIdList;
}

char* ConvertClassIdListToJson(DynamicList* classIdList){
    char *jsonString = malloc(sizeof(char) * 1000);
    char *jsonStringTemp = malloc(sizeof(char) * 1000);
    jsonString[0] = '\0';
    jsonStringTemp[0] = '\0';
    strcat(jsonString, "[");
    for (int i = 0; i < classIdList->size; i++) {
        char *classId = (char *) getElement(classIdList, i);
        sprintf(jsonStringTemp,
                "\"%s\",",
                classId);
        strcat(jsonString, jsonStringTemp);
    }
    jsonString[strlen(jsonString) - 1] = '\0';
    strcat(jsonString, "]");
    free(jsonStringTemp);
    return jsonString;
}

DynamicList* ConvertJsonToClassIdList(char* jsonString) {
    char *start = strchr(jsonString, '[');
    char *end = strrchr(jsonString, ']');

    if (start && end && start < end) {
        memmove(start, start + 1, end - start);
        memmove(end - 1, end + 1, strlen(end) + 1);
    }
    printf("Init JsonString: %s\n", jsonString);
    char *tokenString;
    char *jsonStringCopy = strdup(jsonString);
    char *startJsonString = jsonStringCopy;
    char *endJsonString = jsonStringCopy;
    DynamicList *classIdList = initializeList(sizeof(char) * 20);
    // TODO: Initialize here
}

char* ConvertStudentRegistrationListToJson(DynamicList* studentRs) {
    char *jsonString = malloc(sizeof(char) * 1000);
    char *jsonStringTemp = malloc(sizeof(char) * 1000);
    jsonString[0] = '\0';
    jsonStringTemp[0] = '\0';
    strcat(jsonString, "[");
    for (int i = 0; i < studentRs->size; i++) {
        Student_R *studentR = (Student_R *) getElement(studentRs, i);
        sprintf(jsonStringTemp,
                "{\"userId\":\"%s\",\"classId\":\"%s\"},",
                studentR->userId, studentR->classId);
        strcat(jsonString, jsonStringTemp);
    }
    jsonString[strlen(jsonString) - 1] = '\0';
    strcat(jsonString, "]");
    free(jsonStringTemp);
    return jsonString;
}

DynamicList* ConvertJsonToStudentRegistrationList(char* jsonString) {
    char *start = strchr(jsonString, '[');
    char *end = strrchr(jsonString, ']');

    if (start && end && start < end) {
        memmove(start, start + 1, end - start);
        memmove(end - 1, end + 1, strlen(end) + 1);
    }
    printf("Init JsonString: %s\n", jsonString);
    char *tokenString;
    char *jsonStringCopy = strdup(jsonString);
    char *startJsonString = jsonStringCopy;
    char *endJsonString = jsonStringCopy;
    DynamicList *studentRs = initializeList(sizeof(Student_R));
    // TODO: Loop forever here
    while (*endJsonString != '\0') {
        if (*endJsonString == '}' && (*(endJsonString + 1) == ',' || *(endJsonString + 1) == '\0')) {
            *endJsonString = '}';
            tokenString = strndup(startJsonString, endJsonString - startJsonString + 1);

            if (startJsonString != jsonStringCopy && *startJsonString == ',') {
                // If there's a leading comma, skip it
                tokenString++;
            }

            printf("Token string: %s\n", tokenString);
            char userId[20];
            char classId[20];
            if (sscanf(tokenString,
                       "{\"userId\":\"%19[^\"]\",\"classId\":\"%19[^\"]\"}",
                       userId, classId) == 2) {
                // Create a new Student_R struct and add it to List
                Student_R *tStudentR = CreateStudentRegistration(userId, classId);
                addToDynamicList(studentRs, tStudentR, sizeof(Student_R));
                free(tStudentR);
                tStudentR = NULL;
            } else {
                printf("Error parsing line: %s\n", tokenString);
            }
//            free(tokenString);
            startJsonString = endJsonString + 1;
        }
        endJsonString++;
    }
    printf("Complete\n");
    return studentRs;
}


// USER ACCOUNT
typedef struct user_account {
    char username[20];
    char password[20];
} User;

char* ConvertUserToString(User* user) {
    char *userString = malloc(sizeof(char) * 100);
    sprintf(userString, "%s-%s", user->username, user->password);
    return userString;
}

User *ConvertStringToUser(char* string) {
    User *user = (User *) malloc(sizeof(User));
    char* username = strtok(string, "-");
    char* password = strtok(NULL, "-");
    strcpy(user->username, username);
    strcpy(user->password, password);
    return user;
}

User *UserAuthorized(char *usernameAndPasswordString, char *userAccountPath) {
    char username[20];
    char password[20];
    // Parse username and password from string
    // Example of string: "username-password"


    if (sscanf(usernameAndPasswordString, "%[^-]-%s", username, password) == 2) {
        FILE *fp;
        fp = fopen(userAccountPath, "r");
        if (fp == NULL) {
            printf("Can not open file\n");
            return false;
        }
        char usernameFromFile[20];
        char passwordFromFile[20];
        while (fscanf(fp, "%s %s", usernameFromFile, passwordFromFile) != EOF) {
            if (strcmp(username, usernameFromFile) == 0) {
                if (strcmp(password, passwordFromFile) == 0) {
                    printf("UserLogin successfully\n");
                    fclose(fp);
                    User *user = (User *) malloc(sizeof(User));
                    strcpy(user->username, username);
                    strcpy(user->password, password);
                    return user;
                } else {
                    printf("--> Error: Wrong password\n");
                    fclose(fp);
                    return NULL;
                }
            }
        }
        printf("--> Error: User not found\n");
        fclose(fp);
        return NULL;
    } else {
        printf("--> Error: Invalid username and password\n");
        return NULL;
    }
}

User *UserLogin(char *username, char *password, char *path) {
    FILE *fp;
    fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Can not open file\n");
        return NULL;
    }
    User *user = (User *) malloc(sizeof(User));
    char usernameFromFile[20];
    char passwordFromFile[20];
    bool isFound = false;
    while (fscanf(fp, "%s %s", usernameFromFile, passwordFromFile) != EOF) {
        isFound = false;
        if (strcmp(username, usernameFromFile) == 0) {
            if (strcmp(password, passwordFromFile) == 0) {
                printf("UserLogin successfully\n");
                strcpy(user->username, username);
                strcpy(user->password, password);
                fclose(fp);
                return user;
            } else {
                isFound = true;
                printf("--> Error: Wrong password\n");
                break;
            }
        }
    }
    if (!isFound) {
        printf("--> Error: User not found\n");
    }
    fclose(fp);
    return NULL;
}

void freeUser(User *user) {
    free(user);
    user = NULL;
}

#endif //NETWORK_PROGRAM_SSM_H
