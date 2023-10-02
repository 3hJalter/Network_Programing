//
// Created by Jalter on 9/21/2023.
//

#ifndef NETWORK_PROGRAM_DAYOFWEEK_H
#define NETWORK_PROGRAM_DAYOFWEEK_H

#include <string.h>
#include <ctype.h>

typedef enum DayOfWeek {
    INVALID = 0,
    Monday = 2,
    Tuesday = 3,
    Wednesday = 4,
    Thursday = 5,
    Friday = 6,
    Saturday = 7,
    Sunday = 8,
} DayOfWeek;

enum DayOfWeek ConvertDayStringToEnum(char *dayString) {
    if (dayString == NULL) {
        return 0;  // Handle NULL input gracefully, or choose another error code.
    }

    // Check if the input is a single digit and within the valid range.
    if (strlen(dayString) == 1 && isdigit(dayString[0])) {
        int day = dayString[0] - '0';
        if (day >= 2 && day <= 8) {
            return (enum DayOfWeek) day;
        }
    }

    // Convert the input to lowercase for case-insensitive matching.
    char lowercase[10];
    int i;
    for (i = 0; dayString[i]; i++) {
        lowercase[i] = (char) tolower(dayString[i]);
    }
    lowercase[i] = '\0';

    if (strcmp(lowercase, "monday") == 0 || strcmp(lowercase, "mon") == 0) return Monday;
    if (strcmp(lowercase, "tuesday") == 0 || strcmp(lowercase, "tue") == 0) return Tuesday;
    if (strcmp(lowercase, "wednesday") == 0 || strcmp(lowercase, "wed") == 0) return Wednesday;
    if (strcmp(lowercase, "thursday") == 0 || strcmp(lowercase, "thu") == 0) return Thursday;
    if (strcmp(lowercase, "friday") == 0 || strcmp(lowercase, "fri") == 0) return Friday;
    if (strcmp(lowercase, "saturday") == 0 || strcmp(lowercase, "sat") == 0) return Saturday;
    if (strcmp(lowercase, "sunday") == 0 || strcmp(lowercase, "sun") == 0) return Sunday;

    // If the input doesn't match any of the accepted formats, return 0.
    return INVALID;
}

char ConvertDayStringToChar(char *dayString) {
    if (dayString == NULL) {
        return '\0';  // Handle NULL input gracefully, or choose another error code.
    }

    // Check if the input is a single digit and within the valid range.
    if (strlen(dayString) == 1 && isdigit(dayString[0])) {
        int day = dayString[0] - '0';
        if (day >= 2 && day <= 8) {
            return dayString[0];
        }
    }

    // Convert the input to lowercase for case-insensitive matching.
    char lowercase[10];
    int i;
    for (i = 0; dayString[i]; i++) {
        lowercase[i] = (char) tolower(dayString[i]);
    }
    lowercase[i] = '\0';

    if (strcmp(lowercase, "monday") == 0 || strcmp(lowercase, "mon") == 0) return '2';
    if (strcmp(lowercase, "tuesday") == 0 || strcmp(lowercase, "tue") == 0) return '3';
    if (strcmp(lowercase, "wednesday") == 0 || strcmp(lowercase, "wed") == 0) return '4';
    if (strcmp(lowercase, "thursday") == 0 || strcmp(lowercase, "thu") == 0) return '5';
    if (strcmp(lowercase, "friday") == 0 || strcmp(lowercase, "fri") == 0) return '6';
    if (strcmp(lowercase, "saturday") == 0 || strcmp(lowercase, "sat") == 0) return '7';
    if (strcmp(lowercase, "sunday") == 0 || strcmp(lowercase, "sun") == 0) return '8';

    // If the input doesn't match any of the accepted formats, return '\0' (null character).
    return '\0';
}

// function to convert enum to string
char *ConvertDayEnumToString(enum DayOfWeek dayOfWeek) {
    switch (dayOfWeek) {
        case Monday:
            return "Monday";
        case Tuesday:
            return "Tuesday";
        case Wednesday:
            return "Wednesday";
        case Thursday:
            return "Thursday";
        case Friday:
            return "Friday";
        case Saturday:
            return "Saturday";
        case Sunday:
            return "Sunday";
        default:
            return "Invalid day";
    }
}

#endif //NETWORK_PROGRAM_DAYOFWEEK_H
