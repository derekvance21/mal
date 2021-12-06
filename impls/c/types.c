#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"


mal_t mal_list(vector_t *vec)
{
    mal_t mal;
    mal.type = LIST;
    mal.val.list = vec;
    return mal;
}
mal_t mal_integer(int integer)
{
    mal_t mal;
    mal.type = INTEGER;
    mal.val.integer = integer;
    return mal;
}

mal_t mal_decimal(float decimal)
{
    mal_t mal;
    mal.type = DECIMAL;
    mal.val.decimal = decimal;
    return mal;
}

mal_t mal_symbol(char *symbol)
{
    mal_t mal;
    
    if (strcmp(symbol, "nil") == 0)
    {    
        mal.type = KEYWORD;
        mal.val.keyword = NIL;
        free(symbol);
    }
    else if (strcmp(symbol, "true") == 0)
    {
        mal.type = KEYWORD;
        mal.val.keyword = TRUE;
        free(symbol);
    }
    else if (strcmp(symbol, "false") == 0)
    {
        mal.type = KEYWORD;
        mal.val.keyword = FALSE;
        free(symbol);
    }
    else
    {
        mal.type = SYMBOL;
        mal.val.symbol = symbol;
    }

    return mal;
}

mal_t mal_string(char *string)
{
    mal_t mal;
    mal.type = STRING;
    mal.val.string = string;
    return mal;
}

mal_t mal_error(char *errmsg)
{
    mal_t mal;
    mal.type = ERROR;
    mal.val.errmsg = errmsg;
    return mal;
}

// func: pointer to function which takes a int (argc) and a void* and returns a void*
mal_t mal_func(mal_t (*func)(int, mal_t*))
{
    mal_t mal;
    mal.type = FUNCTION;
    mal.val.func = func;
    return mal;
}