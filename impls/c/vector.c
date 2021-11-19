#include <stdlib.h>
#include <stdio.h>
#include "vector.h"

//TODO: This is a generic vector. Implement mal_t and char vectors (mal_list and string) separately to simplify usage

const int DEFAULT_CAPACITY = 1;

vector_t *vector_init(int item_size)
{
    vector_t *vec = malloc(sizeof(vector_t));
    if (!vec)
    {
        perror("error allocating vector memory");
        exit(1);
    }
    vec->items = calloc(DEFAULT_CAPACITY, item_size);
    if (!vec->items)
    {
        perror("error allocating vector.items memory");
        exit(1);
    }
    vec->len = 0;
    vec->capacity = DEFAULT_CAPACITY;
    vec->item_size = item_size;
    return vec;
}

int vector_push(vector_t *vec)
{
    if (vec->len >= vec->capacity)
    {
        vec->capacity *= 2;
        vec->items = reallocarray(vec->items, vec->capacity, vec->item_size);
        if (!vec->items)
        {
            perror("error allocating vector.items memory");
            exit(1);
        }
    }
    return vec->len++;
}

void vector_free(vector_t *vec)
{
    free(vec->items);
    free(vec);
}
