#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

void vector_init(Vector* vector) {
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}

void vector_push_back(Vector* vector, int element) {
    if (vector->size >= vector->capacity) {
        vector->capacity = (vector->capacity == 0) ? 1 : vector->capacity * 2;
        vector->data = (int*)realloc(vector->data, vector->capacity * sizeof(int));
    }

    vector->data[vector->size] = element;
    vector->size++;
}

int vector_get(const Vector* vector, size_t index) {
    if (index < vector->size) {
        return vector->data[index];
    }

    printf("Error: Index out of bounds\n");
    exit(1);
}

void vector_free(Vector* vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
}
