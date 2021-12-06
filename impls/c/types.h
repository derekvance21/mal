#ifndef TYPES_H
#define TYPES_H

#include "vector.h"
#include "env.h"


enum MalType
{
    KEYWORD,
    LIST,
    INTEGER,
    DECIMAL,
    SYMBOL,
    STRING,
    ERROR,
    FUNCTION,
    CLOSURE,
};

enum Keyword
{
    TRUE,
    FALSE,
    NIL,
};

typedef struct Closure closure_t;

typedef struct Mal
{
    enum MalType type;
    union {
        int integer;
        float decimal;
        char *symbol;
        char *string;
        char *errmsg;
        enum Keyword keyword;
        vector_t *list;
        struct Mal (*func)(int, struct Mal*);
        closure_t *closure;
    } val;
} mal_t;

// closures are just env maintainers, really
// when a closure is eval'd, the bindings get set in the env to the arguments applied to the closure
struct Closure
{
    int argc;
    mal_t *params; // parameters to closure application, vector_t<mal_t>
    mal_t body; // body of closure
    env_t *outer; // outer environment of closure; env_t* needed?
};

// TODO: Generic mal_free(mal_t mal) function that switches on the type and frees alloc'd resources as necessary
// ex: symbol, string need their char* to be free'd, list needs vector_t to be free'd

mal_t mal_list(vector_t *vec);
mal_t mal_integer(int integer);
mal_t mal_decimal(float decimal);
mal_t mal_symbol(char *symbol);
mal_t mal_string(char *string);
mal_t mal_error(char *errmsg);
mal_t mal_func(mal_t (*func)(int, mal_t*));
mal_t mal_closure(int argc, mal_t *params, mal_t body, env_t *outer);


#endif