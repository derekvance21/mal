#ifndef READER_H
#define READER_H

#include "types.h"

struct reader;

typedef struct reader
{
    vector_t *tokens;
    int pos;
    char* (*next)(struct reader *self);
    char* (*peek)(struct reader *self);
} reader_t;

mal_t read_str(char *in);
mal_t read_form(reader_t *reader);
mal_t read_list(reader_t *reader);
mal_t read_atom(reader_t *reader);
vector_t *tokenize(char *in);
reader_t* reader_init(char *in);
void reader_free(reader_t *reader);
char* peek(reader_t *self);
char* next(reader_t *self);


#endif