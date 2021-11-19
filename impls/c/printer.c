#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "printer.h"
#include "types.h"

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
            char *element = pr_str(((mal_t *)mal.val.list->items)[i]);
            list_strlen += strlen(element);

            // similar thing is happening here?
            int pos = vector_push(elements);
            ((char **)elements->items)[pos] = element;
        }
        // need room for the entire length of the n elements, plus n - 1 spaces, plus null byte, plus 2 parentheses
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
            free(element_str);
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
    case ATOM:
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