#include <stdio.h>
#include "types.h"


mal_t mal_atom(mal_t *atom)
{
    mal_t mal;
    mal.type = ATOM;
    mal.val.atom = atom;
    return mal;
}

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
    mal.type = SYMBOL;
    mal.val.symbol = symbol;
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

