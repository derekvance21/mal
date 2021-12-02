#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "types.h"
#include "reader.h"

vector_t *tokenize(char *in)
{
    regex_t regex;
    int reti;
    regmatch_t pmatch[2];
    char errbuf[1024];

    vector_t *tokens = vector_init(sizeof(char*));

    size_t in_len = strlen(in);

    // if I leave the \\[\\] (or '[]', it doesn't matter) it won't match correctly at all
    reti = regcomp(&regex, "[[:space:],]*(~@|[{}()'`~^@]|\"(\\\\.|[^\\\"])*\"?|;.*|[^[:space:]{}('\"`,;)]*)", REG_EXTENDED);

    if (reti)
    {
        regerror(reti, &regex, errbuf, sizeof(errbuf));
        fprintf(stderr, "Could not compile regex: %s\n", errbuf);
        exit(1);
    }

    int i;
    for (i = 0; i < in_len; i = i)
    {
        // pmatch[0] == consumed characters
        // pmatch[1] == captured token
        reti = regexec(&regex, in + i, 2, pmatch, 0);

        if (reti)
        {
            regerror(reti, &regex, errbuf, sizeof(errbuf));
            fprintf(stderr, "Could not execute regex: %s\n", errbuf);
            exit(1);
        }

        int token_len = pmatch[1].rm_eo - pmatch[1].rm_so;

        char *token = malloc(1 + token_len);
        strncpy(token, in + i + pmatch[1].rm_so, token_len);
        token[token_len] = '\0';
        
        // I found an incredibly weird bug here:
        // if you index into items with a call to vector_push(tokens),
        // the 3rd, 5th, 9th, ... elements will be null, but if you provide a variable for it's return value,
        // and use that, the elements will be added correctly
        int new_pos = vector_push(tokens);
        ((char**) tokens->items)[new_pos] = token;
        // printf("vector[%d]: %s - expected: %s\n", tokens->len - 1, ((char**) tokens->items)[tokens->len - 1], token);

        i += pmatch[1].rm_eo;
    }

    regfree(&regex);
    return tokens;
}

char* next(reader_t *self)
{
    return ((char**) self->tokens->items)[self->pos++];
}

char* peek(reader_t *self)
{
    char** tokens_strs = (char**) self->tokens->items;
    char *token = tokens_strs[self->pos];
    return token;
}

reader_t* reader_init(char *in)
{
    reader_t *reader = malloc(sizeof(reader_t));
    reader->tokens = tokenize(in);
    reader->pos = 0;
    reader->next = next;
    reader->peek = peek;
    return reader;
}

void reader_free(reader_t *reader)
{
    vector_free(reader->tokens);
    free(reader);
}

mal_t read_list(reader_t *reader)
{
    vector_t *elements = vector_init(sizeof(mal_t));
    char *token;
    // advance the reader past the opening '(', freeing token
    free(reader->next(reader));

    while (strcmp(token = reader->peek(reader), ")") != 0)
    {
        mal_t element = read_form(reader);
        if (element.type == ERROR)
        {
            return element;
        }

        int pos = vector_push(elements);
        ((mal_t*)elements->items)[pos] = element;

        // check if out of tokens (no matching closing parentheses)
        if (reader->pos >= reader->tokens->len) 
        {
            return mal_error("Syntax Error: missing a matching closing parentheses");
        }
    }

    // advance the reader past the closing ')', freeing token
    free(reader->next(reader));
    return mal_list(elements);
}

mal_t read_atom(reader_t *reader)
{
    char *token = reader->next(reader);
    if (token[0] == '"')
    {
        return mal_string(token);
    }
    else
    {
        // see if it's an integer
        char *endptr;
        int val = strtol(token, &endptr, 10);

        // didn't reach the end of token, so it's not an integer
        if (*endptr != '\0')
        {
            return mal_symbol(token);
        }
        // it's an integer, so free the token char*, because now we'll just use int val
        else 
        {
            free(token);
            return mal_integer(val);
        }

    }
}

mal_t read_comment(reader_t *reader)
{
    free(reader->next(reader));
    return mal_symbol("nil");
}

mal_t read_form(reader_t *reader)
{
    char *token = reader->peek(reader);
    switch (token[0])
    {
        case '(':
        {
            mal_t list = read_list(reader);
            return list;
        }
        case ';':
            return read_comment(reader);
        default:
            return read_atom(reader);
    }
}

mal_t read_str(char *in)
{
    reader_t *reader = reader_init(in);
    mal_t ast = read_form(reader);
    reader_free(reader);
    return ast;
}
