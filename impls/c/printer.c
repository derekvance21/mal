#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "printer.h"
#include "types.h"

char *pr_keyword(enum Keyword keyword)
{
    switch (keyword)
    {
    case FALSE:
        return "false";
    case TRUE:
        return "true";
    case NIL:
        return "nil";
    default:
        return "*not a keyword*";
    }
}

char* pr_str(mal_t mal)
{
    switch (mal.type)
    {
    case SYMBOL:
        return mal.val.symbol;
    case INTEGER:
    {
        // 11 characters is enough for int datatype
        char buf[11];
        int num = snprintf(buf, 11, "%d", mal.val.integer);
        if (num >= 11)
        {
            fprintf(stderr, "error printing integer: %d, need %d character long buffer\n", mal.val.integer, num);
            exit(1);
        }
        char *str = malloc(num + 1); // extra byte for null character, because snprintf returns number of non-null byte characters 
        strcpy(str, buf);
        return str;
    }
    case LIST:
    {
        vector_t *elements = vector_init(sizeof(char *));
        int list_strlen = 0;
        int i;
        for (i = 0; i < mal.val.list->len; ++i)
        {
            char *element = pr_str(((mal_t*)mal.val.list->items)[i]);
            list_strlen += strlen(element);

            int pos = vector_push(elements);
            ((char **)elements->items)[pos] = element;
        }
        // need room for the entire length of the n elements, plus n - 1 spaces, plus null byte, plus 2 parentheses
        // this is the only string I need to leave malloc'd, the rest need to be free'd (symbol, string, others?...) 
        char *list_str = malloc(list_strlen + (elements->len - 1) + 3);
        int list_str_start = 0;
        list_str[list_str_start++] = '(';
        for (i = 0; i < elements->len; ++i)
        {
            // space delimiter prepends non-0th element
            if (i != 0)
            {
                list_str[list_str_start++] = ' ';
            }
            char *element_str = ((char **)elements->items)[i];
            strcpy(list_str + list_str_start, element_str);
            list_str_start += strlen(element_str);
            // this should be every element_str that needs to be free'd
            // integer mallocs in pr_str[INTEGER], symbol and string malloc'd in tokenize, list mallocs in pr_str[LIST]
            // the only one that doesn't (ATM) is ERROR, which uses string literals. This may change with dynamic error messaging
            mal_t element_mal = ((mal_t*)mal.val.list->items)[i];
            // TODO: In future, mal_error's should have dynamically allocated error messages, so this will be deleted
            if (element_mal.type != ERROR && element_mal.type != KEYWORD)
            {
                free(element_str);
            }
        }
        list_str[list_str_start++] = ')';
        list_str[list_str_start] = '\0';
        vector_free(elements);
        return list_str;
    }
    case STRING:
        return mal.val.string;
    case ERROR:
        return mal.val.errmsg;
    case KEYWORD:
        return pr_keyword(mal.val.keyword);
    case DECIMAL:
    default:
    {
        char *msg = "*unsupported type*";
        char *ret = malloc(sizeof(msg));
        strcpy(ret, msg);
        return ret;
    }
    }
}