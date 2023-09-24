//
// Created by Jalter on 9/21/2023.
//
#ifndef NETWORK_PROGRAM_DS_H
#define NETWORK_PROGRAM_DS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dynamic list, which the same as List in Java or C#
typedef struct {
    void *data;
    size_t size;
    size_t capacity;
    size_t elementSize;
} DynamicList;

// Function to initialize the list and return a pointer to the new list
DynamicList *initializeList(size_t elementSize) {
    DynamicList *list = (DynamicList *)malloc(sizeof(DynamicList));
    if (list == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        exit(1);
    }
    list->capacity = 4; // initial capacity
    list->elementSize = elementSize;
    list->data = malloc(list->capacity * elementSize);
    if (list->data == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        free(list);
        exit(1);
    }

    list->size = 0;
    return list;
}

// Get element at index
void *getElement(DynamicList *list, size_t index) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of range\n");
        return NULL;
    }
    return (char *)list->data + (index * list->elementSize);
}

// find element by its data
void *findElement(DynamicList *list, const void *element) {
    size_t index = 0;
    while (index < list->size) {
        void *current = (char *)list->data + (index * list->elementSize);
        if (memcmp(current, element, list->elementSize) == 0) {
            return current;
        }
        index++;
    }
    return NULL;
}


// Remove element at index
void removeElementByIndex(DynamicList *list, size_t index) {
    if (index >= list->size) {
        fprintf(stderr, "Index out of range\n");
        return;
    }
    void *target = (char *)list->data + (index * list->elementSize);
    void *source = (char *)target + list->elementSize;
    size_t size = (list->size - index - 1) * list->elementSize;
    memmove(target, source, size);
    list->size--;
}

// Remove element
void removeElement(DynamicList *list, const void *element) {
    size_t index = 0;
    while (index < list->size) {
        void *current = (char *)list->data + (index * list->elementSize);
        if (memcmp(current, element, list->elementSize) == 0) {
            removeElementByIndex(list, index);
            return;
        }
        index++;
    }
    fprintf(stderr, "Element not found\n");
}

// Remove last element
void removeLastElement(DynamicList *list) {
    if (list->size == 0) {
        fprintf(stderr, "List is empty\n");
        return;
    }
    list->size--;
}

// Function to add an element to the list and increase its size if needed
void addToDynamicList(DynamicList *list, const void *element, size_t elementSize) {
    if (list->elementSize != elementSize) {
        fprintf(stderr, "Input element size (%zu) is not equal to list element size (%zu)\n", elementSize, list->elementSize);
        return;
    }

    if (list->size >= list->capacity) {
        // If the list is full, double the list size using re-alloc
        size_t new_capacity = list->capacity * 2;
        void *new_data = realloc(list->data, new_capacity * list->elementSize);
        if (new_data == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(1);
        }
        list->data = new_data;
        list->capacity = new_capacity;
    }

    // Add new element to the list
    void *target = (char *)list->data + (list->size * list->elementSize);
    memcpy(target, element, list->elementSize);
    list->size++;
}

// Function to release list memory when no longer needed
// Currently work list the data is not a pointer point to another pointer
// For example: if data is also a DynamicList pointer, this function will not work
// Then you need to customize the free function for that data type before call this function
void freeDynamicList(DynamicList *list) {
    free(list->data);
    list->data = NULL;
    free(list);
    list = NULL;
}

#endif //NETWORK_PROGRAM_DS_H

