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
    FUNCTION = 128,
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
        struct Mal (*func)(int, struct Mal*);
    } val;
} mal_t;

// TODO: Generic mal_free(mal_t mal) function that switches on the type and free's alloc'd resources as necessary
// ex: symbol, string need their char* to be free'd, list needs vector_t to be free'd

mal_t mal_atom(mal_t *atom);
mal_t mal_list(vector_t *vec);
mal_t mal_integer(int integer);
mal_t mal_decimal(float decimal);
mal_t mal_symbol(char *symbol);
mal_t mal_string(char *string);
mal_t mal_error(char *errmsg);
mal_t mal_func(mal_t (*func)(int, mal_t*));


#endif