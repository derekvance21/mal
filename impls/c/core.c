#include "types.h"
#include "core.h"

mal_t add(int argc, mal_t *argv)
{
    int sum = 0;
    int i;
    for (i = 0; i < argc; ++i)
    {
        mal_t mal = argv[i];
        if (mal.type != INTEGER)
        {
            return mal_error("Error: cannot add a non-integer with +");
        }
        sum += mal.val.integer;
    }
    return mal_integer(sum);
}

mal_t sub(int argc, mal_t *argv)
{
    if (argc == 0)
    {
        return mal_error("Error: parity mismatch: proc '- requires at least 1 argument");
    }
    mal_t minuend = argv[0];
    if (minuend.type != INTEGER)
    {
        return mal_error("Error: cannot subtract a non-integer with -");
    }
    if (argc == 1)
    {
        return mal_integer(0 - minuend.val.integer);
    }

    int diff = minuend.val.integer;
    int i;
    for (i = 1; i < argc; ++i)
    {
        mal_t subtrahend = argv[i];
        if (subtrahend.type != INTEGER)
        {
            return mal_error("Error: cannot subtract a non-integer with -");
        }
        diff -= subtrahend.val.integer;
    }
    return mal_integer(diff);
}

mal_t mul(int argc, mal_t *argv)
{
    int prod = 1;
    int i;
    for (i = 0; i < argc; ++i)
    {
        mal_t mal = argv[i];
        if (mal.type != INTEGER)
        {
            return mal_error("Error: cannot multiply a non-integer with *");
        }
        prod *= mal.val.integer;
    }
    return mal_integer(prod);
}

// div is a function in stdlib that divides, producing div_t struct with quotient and remainder (could use it)
mal_t divide(int argc, mal_t *argv)
{
    if (argc == 0)
    {
        return mal_error("Error: parity mismatch: proc '/ requires at least 1 argument");
    }
    mal_t dividend = argv[0];
    if (dividend.type != INTEGER)
    {
        return mal_error("Error: cannot divide a non-integer with /");
    }
    if (argc == 1)
    {
        return mal_integer(1 / dividend.val.integer);
    }

    int quot = dividend.val.integer;
    int i;
    for (i = 1; i < argc; ++i)
    {
        mal_t divisor = argv[i];
        if (divisor.type != INTEGER)
        {
            return mal_error("Error: cannot divide a non-integer with /");
        }
        quot /= divisor.val.integer;
    }
    return mal_integer(quot);
}

mal_t eq(int argc, mal_t *argv)
{
    if (argc == 0)
    {
        return mal_error("Error: parity mismatch: proc '/ requires at least 1 argument");
    }
    mal_t comp = argv[0];
    if (comp.type != INTEGER)
    {
        return mal_error("Error: cannot compare a non-integer with =");
    }

    int i;
    for (i = 1; i < argc; ++i)
    {
        mal_t term = argv[i];
        if (term.type != INTEGER)
        {
            return mal_error("Error: cannot compare a non-integer with =");
        }

        if (comp.val.integer != term.val.integer)
        {
            return mal_keyword(FALSE);
        }
    }

    return mal_keyword(TRUE);
}


// evaluate the arguments and make them the elements of a mal list
mal_t list(int argc, mal_t *argv)
{
    vector_t *elements = vector_init(sizeof(mal_t));
    int i;
    for (i = 0; i < argc; ++i)
    {
        int pos = vector_push(elements);
        ((mal_t*)elements->items)[pos] = argv[i];
    }

    return mal_list(elements);
}

mal_t is_list(int argc, mal_t *argv)
{
    if (argc != 1)
    {
        return mal_error("Error: list? expects one argument");
    }

    mal_t arg = argv[0];
    if (arg.type == LIST)
    {
        return mal_keyword(TRUE);
    }
    else
    {
        return mal_keyword(FALSE);
    }
}

mal_t is_empty(int argc, mal_t *argv)
{
    if (argc != 1)
    {
        return mal_error("Error: empty? expects one argument");
    }

    mal_t arg = argv[0];
    if (arg.type != LIST)
    {
        return mal_error("Error: empty? expects a list as its argument");
    }

    if (arg.val.list->len == 0)
    {
        return mal_keyword(TRUE);
    }
    else
    {
        return mal_keyword(FALSE);
    }
}

mal_t length(int argc, mal_t *argv)
{
    if (argc != 1)
    {
        return mal_error("Error: length expects one argument");
    }

    mal_t arg = argv[0];
    if (arg.type != LIST)
    {
        return mal_error("Error: length expects a list as its argument");
    }

    return mal_integer(arg.val.list->len);
}