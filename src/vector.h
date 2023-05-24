#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    int* data;
    size_t size;
    size_t capacity;
} Vector;

void vector_init(Vector* vector);
void vector_push_back(Vector* vector, int element);
int vector_get(const Vector* vector, size_t index);
void vector_free(Vector* vector);

#endif
