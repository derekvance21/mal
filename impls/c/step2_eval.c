#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "types.h"
#include "reader.h"
#include "printer.h"

mal_t eval_ast(mal_t ast)
{
    return ast;
}

mal_t READ(char *in)
{
    return read_str(in);
}

mal_t EVAL(mal_t ast)
{
    return eval_ast(ast);
}

char* PRINT(mal_t ast)
{
    return pr_str(ast);
}

char* rep(char *in)
{
    return PRINT(EVAL(READ(in)));
}

int main(int argc, char *argv[])
{
    char *line;
    for (;;) {
        line = readline("> ");
        if (!line) {
            printf("\n");
            break;
        }
        if (*line) {
            add_history(line);
            printf("%s\n", rep(line));
        }
        free(line);
    }
}