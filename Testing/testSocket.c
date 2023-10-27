#include <stdio.h>
#include <string.h>
#include <malloc.h>

int main() {
    char* jsonString = "{\"courseId\":\"IT3080\",\"courseName\":\"Computer Network\",\"dayStudy\":\"Thursday\",\"startAt\":\"3\",\"endAt\":\"6\",\"weekStudy\":\"22,25-31,33-40\",\"room\":\"TC-502\"},{\"courseId\":\"IT3080\",\"courseName\":\"Computer Network\",\"dayStudy\":\"Thursday\",\"startAt\":\"3\",\"endAt\":\"6\",\"weekStudy\":\"22,25-31,33-40\",\"room\":\"TC-502\"}";

    char* tokenString;
    char* jsonStringCopy = strdup(jsonString); // Make a copy to avoid modifying the original string

    char* startJsonString = jsonStringCopy;
    char* endJsonString = jsonStringCopy;

    while (*endJsonString != '\0') {
        if (*endJsonString == '}' && (*(endJsonString + 1) == ',' || *(endJsonString + 1) == '\0')) {
            *endJsonString = '}';
            tokenString = strndup(startJsonString, endJsonString - startJsonString + 1);

            if (startJsonString != jsonStringCopy && *startJsonString == ',') {
                // If there's a leading comma, skip it
                tokenString++;
            }

            printf("Token string: %s\n", tokenString);
//            free(tokenString);
            startJsonString = endJsonString + 1;
        }
        endJsonString++;
    }
    printf("Complete\n");
    free(jsonStringCopy);

    return 0;
}
