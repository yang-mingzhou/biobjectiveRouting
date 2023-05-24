#include <stdio.h>
#include "vector.h"

int main() {
    Vector numbers;
    vector_init(&numbers);

    vector_push_back(&numbers, 10);
    vector_push_back(&numbers, 20);
    vector_push_back(&numbers, 30);

    printf("Size: %zu\n", numbers.size);
    printf("Capacity: %zu\n", numbers.capacity);

    for (size_t i = 0; i < numbers.size; i++) {
        printf("%d ", vector_get(&numbers, i));
    }
    printf("\n");

    vector_free(&numbers);

    return 0;
}
