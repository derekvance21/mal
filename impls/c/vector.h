#ifndef VECTOR_H
#define VECTOR_H


typedef struct vector
{
    void *items;
    int len;
    int capacity;
    int item_size;
} vector_t;

vector_t* vector_init(int item_size);
int vector_push(vector_t *vec);
void vector_free(vector_t *vec);


#endif