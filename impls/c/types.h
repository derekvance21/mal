#ifndef TYPES_H
#define TYPES_H

#include "vector.h"


typedef enum
{
    ATOM = 1,
    LIST = 2,
    INTEGER = 4,
    DECIMAL = 8,
    SYMBOL = 16,
    STRING = 32,
    ERROR = 64,
} MalType;

typedef struct Mal
{
    MalType type;
    union {
        struct Mal *atom;
        vector_t *list;
        int integer;
        float decimal;
        char *symbol;
        char *string;
        char *errmsg;
    } val;
} mal_t;

mal_t mal_atom(mal_t *atom);
mal_t mal_list(vector_t *vec);
mal_t mal_integer(int integer);
mal_t mal_decimal(float decimal);
mal_t mal_symbol(char *symbol);
mal_t mal_string(char *string);
mal_t mal_error(char *errmsg);


#endif