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
    mal_t result;
    result.type = INTEGER;
    result.val.integer = sum;
    return result;
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
    mal_t result;
    result.type = INTEGER;
    result.val.integer = diff;
    return result;
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
    mal_t result;
    result.type = INTEGER;
    result.val.integer = prod;
    return result;
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
    mal_t result;
    result.type = INTEGER;
    result.val.integer = quot;
    return result;
}
